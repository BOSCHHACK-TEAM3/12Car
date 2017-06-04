import http.client, urllib.request, urllib.parse, urllib.error, base64

headers = {
    # Request headers
    'Content-Type': 'application/json',
    'Ocp-Apim-Subscription-Key': '{fff72c4d03e6462199e376449a16cfc0}',
}

params = urllib.parse.urlencode({
})

try:
    conn = http.client.HTTPSConnection('api.cognitive.azure.cn')
    conn.request("PUT", "/face/v1.0/persongroups/{personGroupId}?%s" % params, "{\"name\":\"group3\",\"userData\":\"the team meanbers of Bosch IOT Hackathon\"}}", headers)
    response = conn.getresponse()
    data = response.read()
    print(data)
    conn.close()
except Exception as e:
    print("[Errno {0}] {1}".format(e.errno, e.strerror))
