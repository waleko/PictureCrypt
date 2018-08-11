import json
filename = 'ErrorsDict.json'

raw = open(filename, 'r')

data = json.load(raw)
print('Existing data:')
for key, value in data.items():
    print(key, value)

print('------------')
print('Type new data')

input_data = input()

while len(input_data):
    key, value = map(str, input_data.split('-'))
    data[key] = value
    input_data = input()

with open(filename, 'w') as f:
    json.dump(data, f, indent=4)
