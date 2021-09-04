import psutil
from win10toast import ToastNotifier
from time import sleep


def check_battery(low,high):
    battery = psutil.sensors_battery()
    percent = battery.percent
    charge = battery.power_plugged
    notice = "目前電量" + str(percent) + "%"

    if percent <= low and charge == False:
        return "電量不足" + notice
    elif percent >= high and charge == True:
        return "電量已充足" + notice


while True:
    toaster = ToastNotifier()
    popput = check_battery(40,80)
    if popput:
        toaster.show_toast("PCbattery",popput,icon_path=None,duration=5)
    sleep(60)


