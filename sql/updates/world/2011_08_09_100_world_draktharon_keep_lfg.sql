-- Fix DrakTharon Keep reward for DF
UPDATE `instance_encounters` SET `creditType` = '0', `creditEntry` = '26632' WHERE `entry` IN ('376', '375');