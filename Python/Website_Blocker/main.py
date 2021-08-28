import time
from datetime import datetime as dt

tempHostPath = "./hosts"
hostPath = "/etc/hosts"
redirectIP = "127.0.0.1"
websiteList = ["www.facebook.com", "facebook.com"]

currentYear = dt.now().year
currentMonth = dt.now().month
currentDay = dt.now().day

while True:
    startHour = 8
    endHour = 16
    startDate = dt(currentYear, currentMonth, currentDay, startHour)
    endDate = dt(currentYear, currentMonth, currentDay, endHour)
    
    if startDate < dt.now() < endDate:
        print("working hours!")

        with open(tempHostPath, "r+") as file:
            content = file.read()            
            for website in websiteList:
                if website in content:
                    pass
                else:
                    file.write(redirectIP + " " + website + "\n")

    else:
        with open(tempHostPath, "r+") as file:
            content = file.readlines()
            file.seek(0)

            for line in content:
                if not any(website in line for website in websiteList):
                    file.write(line)

            file.truncate()

        print("fun hours!")
    
    time.sleep(1)
     