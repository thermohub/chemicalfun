# ChemicalFun

ChemicalFun is a C++ library (Python and C++ API) for generating balanced chemical reactions and for parsing and calculating properties of chemical formulas.

## Installation using Conda

ThermoFun can be easily installed using [Conda](https://conda.io/docs/) package manager. If you have Conda installed, first add the conda-forge channel by executing 

```
#!bash
conda config --add channels conda-forge
```

install ChemicalFun by executing the following command:

```
#!bash
conda install chemicalfun
```

Conda can be installed from [Miniconda](https://conda.io/miniconda.html).

## Compiling the C++ library

### Install Dependencies (if not using Conda environment)

The thermofun library uses nlohmann/json.hpp as thirdparty dependency to parse database files in json format. To install the header only json library in a terminal ```~/thermofun$``` execute the following: 

```
#!bash
sudo ./install-dependencies.sh
```

On Windows run the following batch file in cmd:

```
install-dependencies.bat
```

### If using conda environment (recommended)

Create the thermofun environnement by executing in ```~/chemicalfun$```:

```
conda devenv
```

then activate the environnement

```
conda activate chemicalfun
```

In the terminal ```(chemicalfun)~/chemicalfun$```, execute the following commands:

```
mkdir build && \
cd build && \
cmake .. && \
make install
``` 

For a local installation, you can specify a directory path for the installed files as follows:

```
#!bash
cmake .. -DCMAKE_INSTALL_PREFIX=/home/username/local/
```

### How to use?

The chemical reactions generator generates all independent balanced chemical reactions for a given list of substances. The library uses the method described by Smith and Missen (1997) to generate reactions of dependent substances from master substances. 

## Example

```python
import chemicalfun as cf

formulas = ['Ca+2', 'CO3-2', 'H+', 'OH-', 'HCO3-', 'CaCO3', 'H2O']

chemicalReactions = cf.ChemicalReactions(formulas)

reactions = chemicalReactions.generateReactions() # returns the reactions list as a list of tuples ('substance', coefficient)

# can be transformed to a list of dictionaries, with reaction substances as keys and the reaction coefficients as values
reactions_dic = [{el[0]: el[1] for el in r} for r in reactions] 

print(chemicalReactions.printReactions())
print(reactions_dic)
```

Output
```
HCO3- = H+ + CO3-2
CaCO3 = CO3-2 + Ca+2
H2O = H+ + OH-

[{'H2O': 1.0, 'OH-': -1.0, 'H+': -1.0},
 {'CO3-2': 1.0, 'H2O': 1.0, 'OH-': -1.0, 'HCO3-': -1.0},
 {'Ca+2': 1.0, 'CO3-2': 1.0, 'CaCO3': -1.0}]
```

The master substances in the reactions are decided based on the order in the given list, with master substances being the first. The number of master substances is decided based on the given substance list and thier elemental composition. To swap the master substances exchange the position in the list. 

```python
formulas = ['Ca+2', 'CO3-2', 'H2O', 'OH-', 'H+', 'HCO3-', 'CaCO3' ] # 'H2O' and 'OH-' are in front of 'H+' and will be set as master 

chemicalReactions = cf.ChemicalReactions(formulas)

reactions = chemicalReactions.generateReactions() 

print(chemicalReactions.printReactions())
print(f'master: {chemicalReactions.masterSubstances()}')
```

Output
```
H+ + OH- = H2O
HCO3- + OH- = H2O + CO3-2
CaCO3 = CO3-2 + Ca+2

master: ['Ca+2', 'CO3-2', 'H2O', 'OH-']
```

By default the algorithm produces dissociation reactions (dissociation of dependent substances). To generate formation reactions, where the dependent substances are on the product side of the reaction: 

```python
reactions = chemicalReactions.generateReactions(formation=True) # formation=True to generate formation reactions

print(chemicalReactions.printReactions())
print(f'master: {chemicalReactions.masterSubstances()}')
```

Output
```
H2O = H+ + OH-
H2O + CO3-2 = HCO3- + OH-
CO3-2 + Ca+2 = CaCO3

master: ['Ca+2', 'CO3-2', 'H2O', 'OH-']
dependent: ['H+', 'HCO3-', 'CaCO3']
```

For more complex cases where element valcences are involved this can be defined using `|3|` after the element symbol e.g. `FeFe|3|2O4`. To generate reactions where each element valence has its own master species use `valence=True` when creating a ChemicalReactions object. 

```python
formulas = [ 'OH-', 'H+', 'Fe+2', 'Fe+3', 'FeO@', 'FeFe|3|2O4', 'O2', 'H2O'] 

chemicalReactions = cf.ChemicalReactions(formulas, valence=True)

reactions = chemicalReactions.generateReactions(formation=True) 

print(chemicalReactions.printReactions())
print(f'master: {chemicalReactions.masterSubstances()}')
print(f'dependent: {chemicalReactions.dependentSubstances()}')
```

Output
```
Fe+2 + OH- = FeO@ + H+
4Fe+3 + 2OH- = O2 + 4Fe+2 + 2H+
H+ + OH- = H2O

master: ['OH-', 'H+', 'Fe+2', 'Fe+3', 'FeFe|3|2O4']
dependent: ['FeO@', 'O2', 'H2O']
```

There are 5 master substances, one for each element with valence: `O|0|`, `O|-2|`, `H|1|`, `Fe|2|`, and `Fe|3|`. Without using the valence option we get

```python
chemicalReactions = cf.ChemicalReactions(formulas)

reactions = chemicalReactions.generateReactions(formation=True) 

print(chemicalReactions.printReactions())
print(f'master: {chemicalReactions.masterSubstances()}')
print(f'dependent: {chemicalReactions.dependentSubstances()}')
```

Output
```
Fe+2 + OH- = FeO@ + H+
2Fe+3 + Fe+2 + 4OH- = FeFe|3|2O4 + 4H+
4Fe+3 + 2OH- = O2 + 4Fe+2 + 2H+
H+ + OH- = H2O

master: ['OH-', 'H+', 'Fe+2', 'Fe+3']
dependent: ['FeO@', 'FeFe|3|2O4', 'O2', 'H2O']
```

To have reactions using substance symbols, provide the symbols list in addition to their formulas when creating a ChemicalReactions object. Pay attention to the order in the two lists and that each formula has a corresponding symbol. 

```python
formulas = ["Ca+2", "Fe+2", "Fe|3|+3", "H+", "OH-", "SO4-2", "CaSO4@", "CaOH+", "FeO@", "HFe|3|O2@", "FeOH+", "Fe|3|OH+2", "H2O@",  "FeS|-2|", "FeS|0|S|-2|", "S|4|O2"]
symbols = ["Ca+2", "Fe+2", "Fe+3", "H+", "OH-", "SO4-2", "CaSO4(aq)", "CaOH+", "FeO(aq)", "HFeO2(aq)", "FeOH+", "FeOH+2", "H2O(aq)",  "Pyrrhotite", "Pyrite", "SO2(gas)"]
chemicalReactions = cf.ChemicalReactions(formulas, symbols)

reactions = chemicalReactions.generateReactions() 

print(chemicalReactions.printReactions())
print(f'master: {chemicalReactions.masterSubstances()}')
print(f'dependent: {chemicalReactions.dependentSubstances()}')
```

Output
```
CaSO4(aq) = SO4-2 + Ca+2
CaOH+ = OH- + Ca+2
FeO(aq) + H+ = OH- + Fe+2
HFeO2(aq) + H+ = 2OH- + Fe+3
FeOH+ = OH- + Fe+2
FeOH+2 = OH- + Fe+3
H2O(aq) = OH- + H+
Pyrrhotite + 4OH- + 8Fe+3 = SO4-2 + 4H+ + 9Fe+2
Pyrite + 8OH- + 14Fe+3 = 2SO4-2 + 8H+ + 15Fe+2
SO2(gas) + 2OH- + 2Fe+3 = SO4-2 + 2H+ + 2Fe+2

master: ['Ca+2', 'Fe+2', 'Fe+3', 'H+', 'OH-', 'SO4-2']
dependent: ['CaSO4(aq)', 'CaOH+', 'FeO(aq)', 'HFeO2(aq)', 'FeOH+', 'FeOH+2', 'H2O(aq)', 'Pyrrhotite', 'Pyrite', 'SO2(gas)']
```






