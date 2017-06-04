import http.client, urllib.request, urllib.parse, urllib.error, base64, json

try:
    fileSave = open('faceoutput.json','r')
    while 1:
        line = fileSave.readline()
        if not line:
            break
        mjson = json.loads(line)
        print(mjson[0]["faceId"])
        #print(line)
    fileSave.close()
except Exception as e:
    print("[Errno {0}] {1}".format(e.errno, e.strerror))
