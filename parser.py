import json

def generate_insert(line):
    var = "db.insert({restaurants {restaurant (values: [{restaurant_id: %d}, {name: \"%s\"}, {cuisine: \"%s\"}, {borough: \"%s\"}]) {address(values: [{building: \"%s\"}, {street: \"%s\"}, {zipcode: %d}])}}})\n" % (int(line['restaurant_id']), line['name'], line['cuisine'], line['borough'], line['address']['building'], line['address']['street'], int(line['address']['zipcode']))
    return var

restaurants = []
with open('restaurants-dataset.json', 'r') as file:
    for line in file:
        restaurants.append(json.loads(line))

for r in restaurants:
    del r['address']['coord']
    del r['grades']

with open('commands', 'w') as file:
    for line in restaurants:
        file.write(generate_insert(line))




