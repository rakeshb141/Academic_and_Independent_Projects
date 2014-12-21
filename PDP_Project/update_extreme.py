import csv

fields = ["STATION_NAME","DATE","PRCP","SNOW","AWND","WT01","WT02","WT03","WT04","WT05","WT06","WT07","WT08","WT09","Extreme"]


r = csv.reader(open('out.csv')) # Here your csv file
lines = [l for l in r]
i=0
for l in lines:
	if i==0:
		i=i+1
		#print ("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n")%("ID",l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14])
		#print ("%s,%s,%s,%s\n")%(l[2],l[3],l[4],l[14])

		continue 
	#print l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14];
	spl = l[0].split(" ")
	l[0] = spl[0]
	for f in range(1,14):
		s = int(l[f])
		if s==-9999:
			l[f]="0" 
	for d in range(5,14):
		e = int(l[d])
		if e>0:
			l[14]="1"
	#print l[0],",",l[1],",",l[2],",",l[3],",",l[4],",",l[5],",",l[6],",",l[7],",",l[8],",",l[9],",",l[10],",",l[11],",",l[12],",",l[13],",",l[14];
	#print ("%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n")%(i,l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],l[12],l[13],l[14])
	print ("%s,%s,%s,%s")%(l[2],l[3],l[4],l[14])

	i=i+1
	#if i==5:
	#	break
