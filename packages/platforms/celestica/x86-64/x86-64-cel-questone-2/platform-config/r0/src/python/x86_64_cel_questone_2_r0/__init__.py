from onl.platform.base import *
from onl.platform.celestica import *

class OnlPlatform_x86_64_cel_questone_2_r0(OnlPlatformCelestica,
                                            OnlPlatformPortConfig_48x10_8x100):
    PLATFORM='x86-64-cel-questone-2-r0'
    MODEL="Questone-2"
    SYS_OBJECT_ID=".2060.1"

    def baseconfig(self):
        onlp_interval_time = 30  # second
        file_path = "/var/opt/interval_time.txt"
        qsfp_quantity = 6
        sfp_quantity = 48
        sfp_i2c_start_bus = 2
        print("Initialize and Install the driver here")
        # for m in [ 'cpld', 'fan', 'psu', 'leds', 'sfp' ]:
        # for m in [ 'cpld' ]:
        #   self.insmod("x86-64-celestica-seastone-2-%s.ko" % m)
        # for m in ['switchboard','baseboard_cpld.mod']:
        # self.insmod("seastone2_baseboard_cpld.mod.ko")
        self.insmod("questone2_switchboard.ko")
        self.insmod("questone2_baseboard_cpld.ko")
        #self.insmod("sff_8436_eeprom.ko")
        self.insmod("optoe.ko")
        self.insmod("mc24lc64t.ko")
        os.system(
            "insmod /lib/modules/`uname -r`/kernel/drivers/char/ipmi/ipmi_devintf.ko")
        os.system(
            "insmod /lib/modules/`uname -r`/kernel/drivers/char/ipmi/ipmi_si.ko")
        os.system(
            "insmod /lib/modules/`uname -r`/kernel/drivers/char/ipmi/ipmi_ssif.ko")
        # os.system("rm /etc/rc*.d/S02onlpd 2> /dev/null")
        # os.system("rm /etc/rc*.d/K02onlpd 2> /dev/null")
        # os.system("rm /etc/rc*.d/S02onlp-snmpd 2> /dev/null")
        # os.system("rm /etc/rc*.d/K02onlp-snmpd 2> /dev/null")
        # eeprom driver
        self.new_i2c_device('24lc64t', 0x56, 1)
        # initialize SFP devices name
        for actual_i2c_port in range(sfp_i2c_start_bus, sfp_i2c_start_bus+(qsfp_quantity+sfp_quantity)):
            port_number = actual_i2c_port - (sfp_i2c_start_bus-1)
            if(port_number <= sfp_quantity):
                #print("echo 'QSFP{1}' > /sys/devices/i2c-{0}/{0}-0050/port_name".format(actual_i2c_port,port_number))
                os.system("echo 'SFP{1}' > /sys/devices/i2c-{0}/{0}-0050/port_name".format(actual_i2c_port,port_number))
            else:
                #print("echo 'SFP{1}' > /sys/devices/i2c-{0}/{0}-0050/port_name".format(actual_i2c_port,port_number-qsfp_quantity))
                os.system("echo 'QSFP{1}' > /sys/devices/i2c-{0}/{0}-0050/port_name".format(actual_i2c_port,port_number-sfp_quantity))
            # self.new_i2c_device('sff8436', 0x50, port)
            # self.new_i2c_device('as5912_54x_sfp%d' % port, 0x51, port+25)
        # new device code instruction
        # def new_device(self, driver, addr, bus, devdir):
        # if not os.path.exists(os.path.join(bus, devdir)):
        #     try:
        #         with open("%s/new_device" % bus, "w") as f:
        #             f.write("%s 0x%x\n" % (driver, addr))
        #     except Exception, e:
        #         print "Unexpected error initialize device %s:0x%x:%s: %s" % (driver, addr, bus, e)
        # else:
        #     print("Device %s:%x:%s already exists." % (driver, addr, bus))
        # def new_i2c_device(self, driver, addr, bus_number):
        # bus = '/sys/bus/i2c/devices/i2c-%d' % bus_number
        # devdir = "%d-%4.4x" % (bus_number, addr)
        # return self.new_device(driver, addr, bus, devdir)

        # def new_i2c_devices(self, new_device_list):
        #     for (driver, addr, bus_number) in new_device_list:
        #         self.new_i2c_device(driver, addr, bus_number)

          ########### initialize I2C bus 0 ###########
        # initialize CPLDs
        # self.new_i2c_devices(
        #     [
        #         ('seastone_2_cpld1', 0x60, 0),
        #         ('seastone_2_cpld2', 0x61, 0),
        #         ]
        #     )

        
        # initialize SFP devices
        # for port in range(1, 49):
        #     self.new_i2c_device('as5912_54x_sfp%d' % port, 0x50, port+25)
        #     self.new_i2c_device('as5912_54x_sfp%d' % port, 0x51, port+25)

        # initialize QSFP devices
        # for port in range(49, 55):
        #     self.new_i2c_device('as5912_54x_sfp%d' % port, 0x50, port+25)
        
        # Script for create interval_time cache.
        if os.path.exists(file_path):
            pass
        else:
            with open(file_path, 'w') as f:  
                f.write("{0}\r\n".format(onlp_interval_time))
            f.close()
        
        return True
