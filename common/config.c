#include "common/config.h"
#include "common/buffers.h"
#include "common/flash.h"
#include "common/build.h"
#include "common/dmacopy.h"
#ifdef FUNCTION_VGA
#include "vga/render.h"
#include "vga/vgaout.h"
#endif
#ifdef FUNCTION_Z80
#include "z80/z80buf.h"
#endif

#include <string.h>

volatile compat_t cfg_machine = MACHINE_AUTO;
volatile compat_t current_machine = MACHINE_AUTO;

#ifdef FUNCTION_Z80
volatile usbmux_t usbmux;
volatile serialmux_t serialmux[2];
volatile wifimode_t wifimode;

uint8_t wifi_ssid[32];
uint8_t wifi_psk[32];

uint32_t wifi_address;
uint32_t wifi_netmask;

uint8_t jd_host[32];
uint16_t jd_port;
#endif

#ifdef FUNCTION_VGA
extern volatile bool userfont;
extern uint8_t character_rom[4096];
extern uint8_t terminal_character_rom[4096];
#endif

extern volatile bool businterface_hold;
extern volatile bool businterface_lockout;

volatile uint16_t cfptr = 0;
volatile uint8_t cfbuf[4096];

uint32_t config_temp[1024];

#define REPLY_OK      0x00
#define REPLY_BUSY    0xBB
#define REPLY_EOF     0x01
#define REPLY_NOFILE  0x02
#define REPLY_EPARAM  0x03
#define REPLY_ECMD    0x04

bool DELAYED_COPY_CODE(parse_config)(uint32_t address) {
    uint32_t *config = (uint32_t*)address;
    int i = 0;

    if(config[0] != NEWCONFIG_MAGIC) {
        return false;
    }

    for(i = 0; i < (CONFIG_SIZE/sizeof(uint32_t)); i++) {
        if(config[i] == NEWCONFIG_EOF_MARKER) return true;

        switch(config[i] & 0x0000FFFF) {
            case CFGTOKEN_HOST_AUTO:
                cfg_machine = MACHINE_AUTO;
                break;
            case CFGTOKEN_HOST_II:
                cfg_machine = MACHINE_II;
                break;
            case CFGTOKEN_HOST_IIE:
                cfg_machine = MACHINE_IIE;
                break;
            case CFGTOKEN_HOST_IIGS:
                cfg_machine = MACHINE_IIGS;
                break;
            case CFGTOKEN_HOST_PRAVETZ:
                cfg_machine = MACHINE_PRAVETZ;
                break;
            case CFGTOKEN_HOST_BASIS:
                cfg_machine = MACHINE_BASIS;
                break;
#ifdef FUNCTION_VGA
            case CFGTOKEN_MONO_00:
                mono_palette = (config[i] >> 16) & 0xF;
                break;
            case CFGTOKEN_TBCOLOR:
                terminal_tbcolor = (config[i] >> 16) & 0xFF;
                break;
            case CFGTOKEN_BORDER:
                terminal_border = (config[i] >> 16) & 0xF;
                break;
#else
            case CFGTOKEN_MUX_LOOP:
                serialmux[(config[i] >> 16) & 1] = SERIAL_LOOP;
                break;
            case CFGTOKEN_MUX_UART:
                serialmux[(config[i] >> 16) & 1] = SERIAL_UART;
                break;
            case CFGTOKEN_MUX_USB:
                serialmux[(config[i] >> 16) & 1] = SERIAL_USB;
                break;
            case CFGTOKEN_MUX_WIFI:
                serialmux[(config[i] >> 16) & 1] = SERIAL_WIFI;
                break;
            case CFGTOKEN_MUX_PRN:
                serialmux[(config[i] >> 16) & 1] = SERIAL_PRINTER;
                break;
            case CFGTOKEN_SER_BAUD:
                sio[(config[i] >> 16) & 1].baudrate = config[i+1];
                break;
//            case CFGTOKEN_USB_HOST:
//                usbmux = USB_HOST_CDC;
//                break;
//            case CFGTOKEN_USB_GUEST:
//                usbmux = USB_GUEST_CDC;
//                break;
//            case CFGTOKEN_USB_MIDI:
//                usbmux = USB_GUEST_MIDI;
//              break;
            case CFGTOKEN_WIFI_AP:
                wifimode = WIFI_AP;
                break;
            case CFGTOKEN_WIFI_CL:
                wifimode = WIFI_CLIENT;
                break;
            case CFGTOKEN_WIFI_SSID:
                memset((char*)wifi_ssid, 0, sizeof(wifi_ssid));
                strncpy((char*)wifi_ssid, (char*)(config+i+1), (config[i] >> 24));
                break;
            case CFGTOKEN_WIFI_PSK:
                memset((char*)wifi_psk, 0, sizeof(wifi_psk));
                strncpy((char*)wifi_psk, (char*)(config+i+1), (config[i] >> 24));
                break;
            case CFGTOKEN_WIFI_IP:
                wifi_address = config[i+1];
                break;
            case CFGTOKEN_WIFI_NM:
                wifi_netmask = config[i+1];
                break;
            case CFGTOKEN_JD_HOST:
                memset((uint8_t*)jd_host, 0, sizeof(jd_host));
                strncpy(jd_host, (char*)(config+i+1), (config[i] >> 24));
                break;
            case CFGTOKEN_JD_PORT:
                jd_port = config[i+1];
                break;
#endif
        }

        // Advance by the number of dwords for this token
        i += (((config[i] >> 24) + 3) >> 2);
    }

    return false;
}

void DELAYED_COPY_CODE(default_config)() {
#ifdef FUNCTION_Z80
    serialmux[0] = SERIAL_USB;
    serialmux[1] = SERIAL_LOOP;
    usbmux = USB_GUEST_CDC;
    wifimode = WIFI_AP;
    strcpy((char*)wifi_ssid, "V2RetroNet");
    strcpy((char*)wifi_psk, "Analog");
#endif
    cfg_machine = MACHINE_AUTO;
}

int DELAYED_COPY_CODE(make_config)(uint8_t rev) {
    int i = 0;

    memset(config_temp, 0, sizeof(config_temp));

    config_temp[i++] = NEWCONFIG_MAGIC;
    config_temp[i++] = CFGTOKEN_REVISION | (((uint16_t)rev) << 16);

    switch(cfg_machine) {
    default:
    case MACHINE_AUTO:
        config_temp[i++] = CFGTOKEN_HOST_AUTO;
        break;
    case MACHINE_II:
        config_temp[i++] = CFGTOKEN_HOST_II;
        break;
    case MACHINE_IIE:
        config_temp[i++] = CFGTOKEN_HOST_IIE;
        break;
    case MACHINE_IIGS:
        config_temp[i++] = CFGTOKEN_HOST_IIGS;
        break;
    case MACHINE_PRAVETZ:
        config_temp[i++] = CFGTOKEN_HOST_PRAVETZ;
        break;
    case MACHINE_BASIS:
        config_temp[i++] = CFGTOKEN_HOST_BASIS;
        break;
    }
#ifdef FUNCTION_Z80
    switch(serialmux[0]) {
    case SERIAL_USB:
        config_temp[i++] = CFGTOKEN_MUX_USB;
        break;
    case SERIAL_UART:
        config_temp[i++] = CFGTOKEN_MUX_UART;
        break;
    case SERIAL_WIFI:
        config_temp[i++] = CFGTOKEN_MUX_WIFI;
        break;
    case SERIAL_PRINTER:
        config_temp[i++] = CFGTOKEN_MUX_PRN;
        break;
    default:
    case SERIAL_LOOP:
        config_temp[i++] = CFGTOKEN_MUX_LOOP;
        break;
    }
    switch(serialmux[1]) {
    case SERIAL_USB:
        config_temp[i++] = CFGTOKEN_MUX_USB | 0x10000;
        break;
    case SERIAL_UART:
        config_temp[i++] = CFGTOKEN_MUX_UART | 0x10000;
        break;
    case SERIAL_WIFI:
        config_temp[i++] = CFGTOKEN_MUX_WIFI | 0x10000;
        break;
    case SERIAL_PRINTER:
        config_temp[i++] = CFGTOKEN_MUX_PRN | 0x10000;
        break;
    default:
    case SERIAL_LOOP:
        config_temp[i++] = CFGTOKEN_MUX_LOOP | 0x10000;
        break;
    }
    switch(usbmux) {
    case USB_HOST_CDC:
        config_temp[i++] = CFGTOKEN_USB_HOST;
        break;
    default:
    case USB_GUEST_CDC:
        config_temp[i++] = CFGTOKEN_USB_GUEST;
        break;
    case USB_GUEST_MIDI:
        config_temp[i++] = CFGTOKEN_USB_MIDI;
        break;
    }

    switch(wifimode) {
    case WIFI_CLIENT:
        config_temp[i++] = CFGTOKEN_WIFI_AP;
        break;
    case WIFI_AP:
        config_temp[i++] = CFGTOKEN_WIFI_CL;
        break;
    }

    config_temp[i] = CFGTOKEN_WIFI_SSID | (((uint32_t)strlen((char*)wifi_ssid)+1) << 24);
    strcpy((char*)(config_temp+i+1), (char*)wifi_ssid);
    i += 1 + (((config_temp[i] >> 24) + 3) >> 2);

    config_temp[i] = CFGTOKEN_WIFI_PSK | (((uint32_t)strlen((char*)wifi_psk)+1) << 24);
    strcpy((char*)(config_temp+i+1), (char*)wifi_psk);
    i += 1 + (((config_temp[i] >> 24) + 3) >> 2);
#endif

#ifdef FUNCTION_VGA
    config_temp[i++] = CFGTOKEN_MONO_00 | ((mono_palette & 0xF) << 20);
    config_temp[i++] = CFGTOKEN_TBCOLOR | ((terminal_tbcolor & 0xFF) << 16);
    config_temp[i++] = CFGTOKEN_BORDER | ((terminal_border & 0xF) << 16);

    // TODO: Font
    //config_temp[i++] = CFGTOKEN_FONTSLOT | ((terminal_font & 0x1F) << 16);
#endif
    config_temp[i++] = NEWCONFIG_EOF_MARKER;

    return i * 4;
}

// Verify config block starts with the appropriate magic, has an end marker,
// and doesn't have any fields that would overflow the block.
bool DELAYED_COPY_CODE(is_config_valid)(uint32_t address) {
    uint32_t *config = (uint32_t*)address;
    int i;

    if(config[0] != NEWCONFIG_MAGIC) return false;

    for(i = 0; i < (CONFIG_SIZE/sizeof(uint32_t)); i++) {
        if(config[i] == NEWCONFIG_EOF_MARKER) return true;

        i += ((config[i] >> 24) + 3) >> 2;
    }

    return false;    
}

// Find and return the config revision number for the block
uint8_t DELAYED_COPY_CODE(get_config_rev)(uint32_t address) {
    uint32_t *config = (uint32_t*)address;
    int i;
    
    for(i = 0; i < (CONFIG_SIZE/sizeof(uint32_t)); i++) {
        if((config[i] & 0x0000FFFF) == CFGTOKEN_REVISION)
            return (config[i] >> 16) & 0xFF;

        if(config[i] == NEWCONFIG_EOF_MARKER) return 0x00;

        i += ((config[i] >> 24) + 3) >> 2;
    }
    
    return 0x00;
}

// Every time we write the config we overwrite the older slot,
// ensuring we don't leave the user without a configuration.
// We increment the revision number each time, wrapping back to 0x00 from 0xFF.
bool DELAYED_COPY_CODE(is_primary_config_newer)() {
    uint8_t a=get_config_rev(FLASH_CONFIG_PRIMARY);
    uint8_t b=get_config_rev(FLASH_CONFIG_SECONDARY);
    
    return (a == ((b+1) & 0xff));
}

bool DELAYED_COPY_CODE(read_config)() {
    if(is_config_valid(FLASH_CONFIG_ONETIME)) {
        internal_flags &= ~IFLAGS_TEST;
        soft_switches |= SOFTSW_TEXT_MODE;
        if(parse_config(FLASH_CONFIG_ONETIME))
            return true;
    }
    if(is_config_valid(FLASH_CONFIG_PRIMARY)) {
        if(!is_config_valid(FLASH_CONFIG_SECONDARY) || (is_primary_config_newer())) {
            if(parse_config(FLASH_CONFIG_PRIMARY))
                return true;
        }
    }
    if(is_config_valid(FLASH_CONFIG_SECONDARY)) {
        if(parse_config(FLASH_CONFIG_SECONDARY))
            return true;
    }

    default_config();
    return false;
}

bool DELAYED_COPY_CODE(write_config)(bool onetime) {
    uint8_t rev = 0xFF;
    bool write_secondary = false;
    bool retval = false;

#ifdef FUNCTION_VGA
    // Disable video output to stop DMA and IRQs
    vga_dpms_sleep();
#endif

    if(is_config_valid(FLASH_CONFIG_PRIMARY) && is_config_valid(FLASH_CONFIG_SECONDARY)) {
        write_secondary = is_primary_config_newer();
        rev = write_secondary ? get_config_rev(FLASH_CONFIG_PRIMARY) : get_config_rev(FLASH_CONFIG_SECONDARY);
    } else if(is_config_valid(FLASH_CONFIG_PRIMARY)) {
        write_secondary = true;
        rev = get_config_rev(FLASH_CONFIG_PRIMARY);
    } else if(is_config_valid(FLASH_CONFIG_SECONDARY)) {
        write_secondary = false;
        rev = get_config_rev(FLASH_CONFIG_SECONDARY);
    }

    if(make_config(rev + 1) <= CONFIG_SIZE) {

        if(onetime) {
            flash_range_erase((FLASH_CONFIG_ONETIME-XIP_BASE), CONFIG_SIZE);
            flash_range_program((FLASH_CONFIG_ONETIME-XIP_BASE), (const uint8_t *)config_temp, CONFIG_SIZE);
        } else if(write_secondary) {
            flash_range_erase((FLASH_CONFIG_SECONDARY-XIP_BASE), CONFIG_SIZE);
            flash_range_program((FLASH_CONFIG_SECONDARY-XIP_BASE), (const uint8_t *)config_temp, CONFIG_SIZE);
        } else {
            flash_range_erase((FLASH_CONFIG_PRIMARY-XIP_BASE), CONFIG_SIZE);
            flash_range_program((FLASH_CONFIG_PRIMARY-XIP_BASE), (const uint8_t *)config_temp, CONFIG_SIZE);
        }
        
        retval = true;
    }

#ifdef FUNCTION_VGA
    // Enable video output now that we are done
    vga_dpms_wake();
#endif

    return retval;
}

#ifdef FUNCTION_VGA
uint8_t DELAYED_COPY_CODE(test_font)(uint16_t param0) {
    int i;

    for(i = 0; i < 4096; i++) {
        character_rom[i] = apple_memory[param0+i];
    }

    return REPLY_OK;
}

uint8_t DELAYED_COPY_CODE(write_font)(uint16_t param0, uint16_t param1) {
    int i;
    
    if(param1 > 0x27) return REPLY_EPARAM;

    // Disable video output to stop DMA and IRQs
    vga_dpms_sleep();

    for(i = 0; i < 4096; i++) {
        character_rom[i] = apple_memory[param0+i];
    }

    flash_range_erase((FLASH_FONT(param1)-XIP_BASE), FONT_SIZE);
    flash_range_program((FLASH_FONT(param1)-XIP_BASE), character_rom, FONT_SIZE);

    // Enable video output now that we are done
    vga_dpms_wake();

    return REPLY_OK;
}
#endif

uint8_t DELAYED_COPY_CODE(cf_readblock)(uint16_t param0) {
    if(param0 >= (FLASH_SIZE/4096)) return REPLY_EPARAM;
    
#ifdef FUNCTION_VGA
    // Disable video output to stop DMA and IRQs
    vga_dpms_sleep();
#endif

    memcpy32((void*)cfbuf, (void*)(XIP_BASE+(param0 * 4096)), 4096);

#ifdef FUNCTION_VGA
    // Enable video output now that we are done
    vga_dpms_wake();
#endif

    return REPLY_OK;
}

uint8_t DELAYED_COPY_CODE(cf_writeblock)(uint16_t param0) {
    // Protect bottom 512K of flash
    if(param0 <  0x080)
        return REPLY_EPARAM;

    if(param0 >= (FLASH_SIZE/4096))
        return REPLY_EPARAM;

#ifdef FUNCTION_VGA
    // Disable video output to stop DMA and IRQs
    vga_dpms_sleep();
#endif

    flash_range_program(param0 * 4096, (void*)cfbuf, 4096);

#ifdef FUNCTION_VGA
    // Enable video output now that we are done
    vga_dpms_wake();
#endif

    return REPLY_OK;
}

uint8_t DELAYED_COPY_CODE(cf_eraseblock)(uint16_t param0) {
    // Protect bottom 512K of flash
    if(param0 <  0x080)
        return REPLY_EPARAM;

    if(param0 >= (FLASH_SIZE/4096))
        return REPLY_EPARAM;

#ifdef FUNCTION_VGA
    // Disable video output to stop DMA and IRQs
    vga_dpms_sleep();
#endif

    flash_range_erase(param0 * 4096, 4096);

#ifdef FUNCTION_VGA
    // Enable video output now that we are done
    vga_dpms_wake();
#endif

    return REPLY_OK;
}

void DELAYED_COPY_CODE(config_handler)() {
    uint8_t retval = 0;
    uint16_t param0, param1, param2;
    int rs = 1;

    if(config_cmdbuf[0] == 0xFF) return;

    param0 = config_cmdbuf[3];
    param0 <<= 8;
    param0 |= config_cmdbuf[2];

    param1 = config_cmdbuf[5];
    param1 <<= 8;
    param1 |= config_cmdbuf[4];

    param2 = config_cmdbuf[7];
    param2 <<= 8;
    param2 |= config_cmdbuf[6];

    config_rpybuf[7] = 0x00;
    config_rpybuf[6] = 0x00;
    config_rpybuf[5] = 0x00;
    config_rpybuf[4] = 0x00;
    config_rpybuf[3] = 0x00;
    config_rpybuf[2] = 0x00;
    config_rpybuf[1] = 0x00;
    config_rpybuf[0] = REPLY_BUSY;

    switch(config_cmdbuf[0]) {
#ifdef FUNCTION_VGA
        case 'C':
            switch(config_cmdbuf[1]) {
                default:
                    retval = REPLY_ECMD;
                    break;
                case 'T':
                    // One-time load of font data (lost at reboot)
                    retval = test_font(param0);
                case 'I':
                    // Save font to flash
                    retval = write_font(param0, param1);
                    break;
            }
            break;
#endif
        case 'H':
            retval = REPLY_OK;
            switch(config_cmdbuf[1]) {
                default:
                    retval = REPLY_ECMD;
                    break;
                case '2':
                    current_machine = MACHINE_II;
                    internal_flags &= ~(IFLAGS_IIE_REGS | IFLAGS_IIGS_REGS);
                    break;
                case 'E':
                    current_machine = MACHINE_IIE;
                    internal_flags &= ~IFLAGS_IIGS_REGS;
                    internal_flags |= IFLAGS_IIE_REGS;
                    break;
                case 'G':
                    current_machine = MACHINE_IIGS;
                    internal_flags |= IFLAGS_IIE_REGS | IFLAGS_IIGS_REGS;
                    break;
#if 0
                case 'B':
                    current_machine = MACHINE_BASIS;
                    internal_flags &= ~(IFLAGS_IIE_REGS | IFLAGS_IIGS_REGS);
                    break;
                case 'P':
                    current_machine = MACHINE_PRAVETZ;
                    internal_flags &= ~(IFLAGS_IIE_REGS | IFLAGS_IIGS_REGS);
                    break;
                case '7':
                    current_machine = MACHINE_AGAT7;
                    internal_flags &= ~(IFLAGS_IIE_REGS | IFLAGS_IIGS_REGS);
                    break;
                case '9':
                    current_machine = MACHINE_AGAT9;
                    internal_flags &= ~(IFLAGS_IIE_REGS | IFLAGS_IIGS_REGS);
                    break;
#endif
            }
            break;

        case 'f':
            // Flash Commands
            // All valid flash commands will reset the cf pointer to 0
            switch(config_cmdbuf[1]) {
                default:
                    retval = REPLY_ECMD;
                    break;
                case 'r':
                    // Read block
                    retval = cf_readblock(param0);
                    cfptr = 0;
                    break;
                case 'w':
                    // Write block
                    retval = cf_writeblock(param0);
                    cfptr = 0;
                    break;
                case 'e':
                    // Erase block
                    retval = cf_eraseblock(param0);
                    cfptr = 0;
                    break;
            }
            break;

        case 'R':
            // Reboot and bypass auto-detection of machine type.
            cfg_machine = current_machine;
            write_config(true);
            flash_reboot();
            break;

        case 'I':
            switch(config_cmdbuf[1]) {
                default:
                    retval = REPLY_ECMD;
                    break;
                case 'H':
                    // Identify Current Host Setting
                    retval = REPLY_OK;
                    switch(current_machine) {
                        case MACHINE_II:
                            config_rpybuf[rs++] = '2';
                            break;
                        case MACHINE_IIE:
                            config_rpybuf[rs++] = 'E';
                            break;
                        case MACHINE_IIGS:
                            config_rpybuf[rs++] = 'G';
                            break;
                        default:
                            config_rpybuf[rs++] = '?';
                            retval = REPLY_EPARAM;
                            break;
                    }
                    break;

                case 'F':
                    // Identify Current Firmware Function
                    retval = REPLY_OK;
#ifdef FUNCTION_VGA
                    config_rpybuf[rs++] = 'V';
                    config_rpybuf[rs++] = 'G';
                    config_rpybuf[rs++] = 'A';
#endif
#ifdef FUNCTION_Z80
                    config_rpybuf[rs++] = 'Z';
                    config_rpybuf[rs++] = '8';
                    config_rpybuf[rs++] = '0';
#endif
                    break;

                case 'h':
                    // Identify Hardware Type
                    retval = REPLY_OK;
                    config_rpybuf[rs++] = 0x02; // V2 Retro Computing
#ifdef ANALOG_GS
                    // AnalogGS Rev 1
                    config_rpybuf[rs++] = 'G';
                    config_rpybuf[rs++] = '1';
#else
                    // Analog Rev 1
                    config_rpybuf[rs++] = 'A';
                    config_rpybuf[rs++] = '1';
#endif
                    break;

                case 'd':
                    // Identify Date Request
                    retval = REPLY_OK;
                    config_rpybuf[rs++] = (BUILDDATE >> 24) & 0xFF;
                    config_rpybuf[rs++] = (BUILDDATE >> 16) & 0xFF;
                    config_rpybuf[rs++] = (BUILDDATE >> 8) & 0xFF;
                    config_rpybuf[rs++] = (BUILDDATE >> 0) & 0xFF;
                    config_rpybuf[rs++] = (BUILDID >> 8) & 0xFF;
                    config_rpybuf[rs++] = (BUILDID >> 0) & 0xFF;
                    break;
            }
            break;
        default:
            // Unrecognized command
            retval = REPLY_ECMD;
    }

    config_cmdbuf[7] = 0xFF;
    config_cmdbuf[6] = 0xFF;
    config_cmdbuf[5] = 0xFF;
    config_cmdbuf[4] = 0xFF;
    config_cmdbuf[3] = 0xFF;
    config_cmdbuf[2] = 0xFF;
    config_cmdbuf[1] = 0xFF;
    config_cmdbuf[0] = 0xFF;
    
    config_rpybuf[0] = retval;
}