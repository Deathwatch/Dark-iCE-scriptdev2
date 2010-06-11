
# Wachen

UPDATE creature_template SET unit_flags=0 WHERE entry=31094;

# Flickwerk

UPDATE creature_template SET dmg_multiplier=12, rank=1 WHERE entry=31099;
UPDATE creature SET spawntimesecs=360 WHERE id=31099;

# citizen of new-avalon

UPDATE creature_template SET dynamicflags=0 WHERE entry=28942;

# Horn of Frostbrood

UPDATE item_template SET ScriptName='item_horn_of_the_frostbrood' WHERE entry=39700;
