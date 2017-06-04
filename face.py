import http.client, urllib.request, urllib.parse, urllib.error, base64
from PIL import Image

headers = {
    # Request headers
    'Content-Type': 'application/octet-stream',
    'Ocp-Apim-Subscription-Key': '2ce1fdcbba3c4f4c996808807f545096',
}

params = urllib.parse.urlencode({
    # Request parameters
    'returnFaceId': 'true',
    'returnFaceLandmarks': 'false',
})

try:
    conn = http.client.HTTPSConnection('api.cognitive.azure.cn')
    
    fileSave = open('test.jpg','rb')
    conn.request("POST", "/face/v1.0/detect?%s" % params, fileSave, headers)
    response = conn.getresponse()
    data = response.read()
    print(data)
    conn.close()
except Exception as e:
    print("[Errno {0}] {1}".format(e.errno, e.strerror))
