import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patheffects as path_effects
import json

with open('mountainsjson', encoding="utf8") as data_file:
    mountains = json.load(data_file)


# Utility function to extract data from country objects.



# Heights of the world's mountains.

hts = np.array([extract(m, "Elevation") for m in mountains])

# Histogram plot of mountain heights.

with plt.xkcd():  # Cueball: "Why?"  Black hat: "Why not?"
    plt.hist(hts, 20, facecolor='cyan', alpha=0.75)
    plt.figure(1)
    print("Histogramoftheheightsofnamedmountains")
    plt.xlabel("Mountainheight")
    plt.ylabel("Mountaincount")
    plt.show(1)

# Ordinary data plot of mountain heights.

plt.figure(2)
pef = [path_effects.SimpleLineShadow(), path_effects.Normal()]
plt.plot(np.sort(hts), '--', path_effects=pef)
print("Heightsofthemountainsoftheworld")
plt.show(2)

with open('countriesjson', encoding="utf-8") as data_file:
    countries = json.load(data_file)

pops = np.array([extract(c, "Population") for c in countries])
gdp = np.array([extract(c, "GDP") for c in countries])
gdppc = gdp / pops
pops_log = np.log(pops) / np.log(10)

# Scatter plot of population against GDP per person.

plt.figure(3)
plt.autoscale(enable=True, axis='x', tight=False)
plt.scatter(pops_log, gdppc, c='g', marker='.')
plt.xlabel('Population')  # LaTeX-style math markup
plt.ylabel('GDPperson')
plt.show(3)
print("ScatterplotofcountriesGDPPersonandopulation")

# Compute the total population of each continent.

continental_pops = {}
for country in countries:
    name, continent = country['Name'], country['Continent']
    pop = extract(country, 'Population', 0)
    continental_pops[continent] = continental_pops.get(continent, 0) + pop

keys = continental_pops.keys()
cp = np.array([continental_pops[continent] for continent in keys])

# Pie plot of continental populations.

plt.figure(4)
plt.axes([1, 1, 1, 1])
plt.pie(cp, labels=keys, explode=[0, 0, .1, 0, 0, .3])
print("Portionofworldpopulationineachcontinent")
plt.show(4)
# plt.savefig("continental_pie.svg")
