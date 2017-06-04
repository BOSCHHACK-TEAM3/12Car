import serial
import urllib.request
import urllib.request
import time
import json
import http.client, urllib.request, urllib.parse, urllib.error, base64, json
from picamera.array import PiRGBArray
from picamera import PiCamera
from PIL import Image
import numpy as np
import binascii

headers = {
    # Request headers
    'Content-Type': 'application/octet-stream',
    'Ocp-Apim-Subscription-Key': '2ce1fdcbba3c4f4c996808807f545096',
}

params = urllib.parse.urlencode({
    # Request parameters
    'returnFaceId': 'true',
})

header = {
    # Request headers
        'Content-Type': 'application/json',
        'Ocp-Apim-Subscription-Key': '2ce1fdcbba3c4f4c996808807f545096',
}


judgecomster=0
#0: whether get the right to the car 1: nopassenger, 2: has a passenger but don't start video; 3 has detect the passenger and the passenger is right; 4 has detect the passenger and the passenger is not
judgeright=0
isright=0
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
url="http://42.159.133.122:8082/mongodb/boschxdk27/latestdata"
camera=PiCamera()
camera.resolution=(320,240)
camera.framerate=60
while True:  
    while judgeright==0:
        request = urllib.request.urlopen(url)
        receive = request.read().decode("utf-8")
        jr=json.loads(receive)
        print(jr['millilux'])
        receivedata=jr['millilux']
        time.sleep(1)
        if receivedata==1:
            judgeright=1
    
    while judgecomster==0:
        if judgeright==1:
            ser.write(bytes('a'.encode('ascii')))
            print('a'.encode('ascii'))
            incoming_byte = ser.read()
            if(incoming_byte):
                incoming_ascii = int.from_bytes(incoming_byte, "big") % 255
                if chr(incoming_ascii)=='f':
                    judgecomster=1
    while judgecomster==1:
        incoming_byte = ser.read()
        if incoming_byte:
            incoming_ascii = int.from_bytes(incoming_byte, "big") % 255
            if chr(incoming_ascii)=='c':
                judgecomster=2
                
    while judgecomster==2:
        print("there is a passenger")
        
        conn = http.client.HTTPSConnection('api.cognitive.azure.cn')
        file_object = open('faceoutput.json', 'w+')            
        fileSave0 = open('su.jpg','rb')
        try:
            conn.request("POST", "/face/v1.0/detect?%s" % params, fileSave0, headers)
            response0 = conn.getresponse()
            data0 = response0.read()
            stmp1="\n"+"\"faceId1\":\""+json.loads(data0.decode('UTF-8'))[0]["faceId"]+"\",\n"
            i=1
            while(i<4):
                camera.capture("test"+str(i)+".jpg")
                fileSave = open('test'+str(i)+'.jpg','rb')
                conn.request("POST", "/face/v1.0/detect?%s" % params, fileSave, headers)
                response = conn.getresponse()
                data = response.read()
                print(data)
                if len(data)>6:
                    stmp2="\"faceId2\":\""+json.loads(data.decode('UTF-8'))[0]["faceId"]+"\"\n"
                    stmp=stmp1+stmp2
                    print(stmp)
                    file_object.write(stmp)
                    conn.request("POST", "/face/v1.0/verify?%s" % params, "{"+stmp+"}", header)
                    response4 = conn.getresponse()                
                    data4 = response4.read().decode('UTF-8')
                    print(data4)
                    print(json.loads(data4)["confidence"])
                    if json.loads(data4)["confidence"]>0.6:
                        judgecomster=3
                        isright=1
                        urllib.request.urlopen("http://42.159.133.122:8082/azure/boschxdk27/2")
                        break
                    i+=1
                conn.close()

        except Exception as e:
                print("[Errno {0}] {1}".format(e.errno, e.strerror))
        finally:
                conn = http.client.HTTPSConnection('api.cognitive.azure.cn')
            
        if isright==0:
            judgecomster=0
            print("Wrong Passenger!")
            urllib.request.urlopen("http://42.159.133.122:8082/azure/boschxdk27/3")
            while judgeright==1:
                ser.write(bytes('b'.encode('ascii')))
                print('b'.encode('ascii'))
                incoming_byte = ser.read()
                if incoming_byte:
                    incoming_ascii = int.from_bytes(incoming_byte, "big") % 255
                    if chr(incoming_ascii)=='f':
                        judgeright=0
    while judgecomster==3:
        incoming_byte = ser.read()
        if incoming_byte:
            incoming_ascii = int.from_bytes(incoming_byte, "big") % 255
            if chr(incoming_ascii)=='n' and judgeright==1:
                #judgecomster=0;
                judgeright=0
                isright=0
                print("the passenger has leaved!")
                urllib.request.urlopen("http://42.159.133.122:8082/azure/boschxdk27/4")
            elif chr(incoming_ascii)=='f':
                judgecomster=0
        if judgeright==0:
            ser.write(bytes('b'.encode('ascii')))
            print('b'.encode('ascii'))
            

