DROP TABLE IF EXISTS `boss_spell_table`;
CREATE TABLE IF NOT EXISTS `boss_spell_table` (
  `entry`        mediumint(8) NOT NULL DEFAULT '0' COMMENT 'Creature entry',
  `spellID_N10`  mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell ID for 10 normal',
  `spellID_N25`  mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell ID for 25 normal',
  `spellID_H10`  mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell ID for 10 heroic',
  `spellID_H25`  mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell ID for 25 heroic',
  `timerMin_N10` mediumint(8) unsigned NOT NULL DEFAULT '15000' COMMENT 'Minimum timer for this spell (in ms.) for 10 normal',
  `timerMin_N25` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Minimum timer for this spell (in ms.) for 25 normal',
  `timerMin_H10` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Minimum timer for this spell (in ms.) for 10 heroic',
  `timerMin_H25` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Minimum timer for this spell (in ms.) for 25 heroic',
  `timerMax_N10` mediumint(8) unsigned NOT NULL DEFAULT '30000' COMMENT 'Maximum timer for this spell (in ms.) for 10 normal',
  `timerMax_N25` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Maximum timer for this spell (in ms.) for 25 normal',
  `timerMax_H10` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Maximum timer for this spell (in ms.) for 10 heroic',
  `timerMax_H25` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Maximum timer for this spell (in ms.) for 25 heroic',
  `data1`        mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Various INT data for this spell or summon for 10 normal',
  `data2`        mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Various INT data for this spell for 25 normal',
  `data3`        mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Various INT data for this spell for 10 heroic',
  `data4`        mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Various INT data for this spell for 25 heroic',
  `locData_x`    float NOT NULL DEFAULT '0' COMMENT 'Location X data for this spell',
  `locData_y`    float NOT NULL DEFAULT '0' COMMENT 'Location Y data for this spell',
  `locData_z`    float NOT NULL DEFAULT '0' COMMENT 'Location Z data for this spell',
  `varData`      mediumint(8) NOT NULL DEFAULT '0' COMMENT 'Special data field for this spell (basepoint for Aura, for example)',
  `StageMask_N`  smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Stage mask for this spell (don\'t used now)',
  `StageMask_H`  smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Stage mask for this spell - heroic mode (don\'t used now)',
  `CastType`     smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Type of cast (by enum BossSpellTableParameters)',
  `isVisualEffect` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Is spell is Visual effect only',
  `isBugged`     tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Is spell bugged and need override',
  `textEntry`    mediumint(8) NOT NULL DEFAULT '0' COMMENT 'Text from script_texts for this spell casting',
  `comment`      text,
  PRIMARY KEY (`entry`,`spellID_N10`,`CastType`),
  INDEX `idx_entry`(`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT 'Boss spell table by /dev/rsa';

-- Insert comments to scriptdev2 boss_spell_table from WOWD database. 
-- Change WOWD database name if you use this!

CREATE ALGORITHM = TEMPTABLE VIEW `commentlist` 
(`entry` ,`spell`, `comment`)
AS SELECT `scriptdev2`.`boss_spell_table`.`entry`,
`spellID_N10`,
CONCAT(`mangos`.`creature_template`.`name`,
' : ',
`mangos`.`wowd_spell`.`SpellName`)
FROM `scriptdev2`.`boss_spell_table`
INNER JOIN `mangos`.`creature_template` ON `mangos`.`creature_template`.`entry` = `scriptdev2`.`boss_spell_table`.`entry` 
INNER JOIN `mangos`.`wowd_spell` ON `mangos`.`wowd_spell`.`id` = `scriptdev2`.`boss_spell_table`.`spellID_N10`;

UPDATE `scriptdev2`.`boss_spell_table` SET `comment` =  (SELECT DISTINCT `commentlist`.`comment`
FROM `commentlist` WHERE `scriptdev2`.`boss_spell_table`.`entry` = `commentlist`.`entry` 
AND `scriptdev2`.`boss_spell_table`.`spellID_N10` = `commentlist`.`spell`
AND `scriptdev2`.`boss_spell_table`.`comment` IS NULL);
DROP VIEW `commentlist`;