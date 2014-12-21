import csv
fields = ["STATION_NAME","DATE","PRCP","SNOW","AWND","WT01","WT02","WT03","WT04","WT05","WT06","WT07","WT08","WT09","Extreme"]

with open("new.csv") as infile, open("out.csv", "w") as outfile:
    #           in Python 2, use open("out.csv", "wb") as outfile:
    r = csv.DictReader(infile)
    w = csv.DictWriter(outfile, fields, extrasaction="ignore")
    w.writeheader()
    for row in r:
        w.writerow(row)
