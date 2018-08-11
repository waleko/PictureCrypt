import json
filename = 'tests.json'

raw = open(filename, 'r')

js = json.load(raw)
print('Existing tests:')
for obj in js['tests']:
    print(obj['data'], obj['image'], obj['expectation'], obj['mode'], obj['key'], obj['bitsUsed'], sep='-')

print('------------')
print('Type new tests')

input_data = input()

arr = []
while len(input_data):
    data, image, expect, mode, key, bitsUsed = map(str, input_data.split('-'))

    obj = {'data':data, 'image':image,'expectation':expect,'mode':int(mode),'key':key,'bitsUsed':int(bitsUsed)}
    arr.append(obj)
    input_data = input()

js['tests'] += arr
with open(filename, 'w') as f:
    json.dump(js, f, indent=4)
