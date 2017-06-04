import http.client, urllib.request, urllib.parse, urllib.error, base64, json

headers = {
    # Request headers
    'Content-Type': 'application/octet-stream',
    'Ocp-Apim-Subscription-Key': 'fff72c4d03e6462199e376449a16cfc0',
}

params = urllib.parse.urlencode({
    # Request parameters
    'returnFaceId': 'true',
})

header = {
    # Request headers
        'Content-Type': 'application/json',
        'Ocp-Apim-Subscription-Key': 'fff72c4d03e6462199e376449a16cfc0',
}

try:
    conn = http.client.HTTPSConnection('api.cognitive.azure.cn')
    file_object = open('faceoutput.json', 'w+')
    #file_object.write("{\n    \"faceIds\":[")
    stmp="\n"
    xlen=4
    for x in range(2,xlen):
        fileSave = open('D:\\17\\intern\\Bosch\\test\\'+str(x)+'.jpg','rb')
        conn.request("POST", "/face/v1.0/detect?%s" % params, fileSave, headers)
        response = conn.getresponse()
        data = response.read()
        print(data)
        stmp+="\"faceId"+str(x-1)+"\":\""+json.loads(data.decode('UTF-8'))[0]["faceId"]+"\""
        if x==xlen-1:
            stmp+="\n"
        else:
            stmp+=",\n"
    print(stmp)
    file_object.write(stmp)
    # file_object.close()
    # personGroupId="weride"
    # conn.request("PUT", "/face/v1.0/persongroups/{"+personGroupId+"}%s" % params, "{\"name\":\"Hackathon03\",\"userData\":\"the team meanbers of Bosch IOT Hackathon\"}}", headers)
    # response2 = conn.getresponse()
    # data2 = response2.read()
    # conn.request("POST", "/face/v1.0/persongroups/"+personGroupId+"/train?%s" % params, "", headers)
    # print(data2)
    # if data2 is "":
    #     body3="{    \n    \"personGroupId\":\"sample_group\",\n"+stmp+"    \"maxNumOfCandidatesReturned\":1,\n    \"confidenceThreshold\": 0.5\n}"
    #     conn.request("POST", "/face/v1.0/identify?%s" % params, body3, headers)
    #     response3 = conn.getresponse()
    #     data3 = response3.read()
    #     print(data3)
    conn.request("POST", "/face/v1.0/verify?%s" % params, "{"+stmp+"}", header)
    response4 = conn.getresponse()
    data4 = response4.read().decode('UTF-8')
    print(json.loads(data4)["isIdentical"])
    if json.loads(data4)["confidence"]:
        #json.loads(data4)["isIdentical"]:
        print("Yes")
    conn.close()
except Exception as e:
    print("[Errno {0}] {1}".format(e.errno, e.strerror))
