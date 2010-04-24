This program is (most likley) only relevant to
Sweden. This because Sweden have separate rules
on how long and heavy a weicle can be compared
to the rest of the European Union/world.


The max weight is calculated using gross weight tables
issued by the Swedish government. These tables (in
electronic form the '../table-*.txt' files is in the
'Resorces/tables.bun' file FS) is used after you know
the distance between the axles.


The 'BK[1-3]' stands for 'Bärklass' (untranslatable
to my knowledge - means roughly 'load classification')
is the classification on the road. A BK1 road is
classified to hold a veicle weighting a maximum 60
metric tons. A BK2 51.4 tons and a BK3 road a maximum
37.5 tons (with some exceptions that my program takes
into account automatically).

But this is dependent on how far the distance between
the front and back axle is...

The max road load is also dependent on how MANY axles
the veicle have. The more axles, the more weight is
allowed.


The max load (the actual max weight of the cargo) is
calculated using the max gross weight minus the
veicle(s) kerb weight (the weight of the veicle,
without cargo, but fully fuled, all liquids and a
driver - the current numbers from the gorvernment
is that it's calculated that the driver weights 70
kilogram).


The term 'veicle' is the truck OR a trailer (with or
without - TYPE1 - front axle). A veicle in the form
truck AND trailer is called 'a veicle train', but for
simplicity, the first term is used.