# SQL Manager for MySQL 5.4.1.42661
# ---------------------------------------
# Host     : localhost
# Port     : 3306
# Database : mu2


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES latin1 */;

SET FOREIGN_KEY_CHECKS=0;

DROP DATABASE IF EXISTS `mu2`;

CREATE DATABASE `mu2`
    CHARACTER SET 'latin1'
    COLLATE 'latin1_swedish_ci';

USE `mu2`;

#
# Structure for the `accounts` table : 
#

CREATE TABLE `accounts` (
  `id` VARCHAR(11) COLLATE latin1_swedish_ci NOT NULL DEFAULT '',
  `password` VARCHAR(11) COLLATE latin1_swedish_ci NOT NULL DEFAULT '',
  `status` TINYINT(1) UNSIGNED ZEROFILL DEFAULT NULL,
  `ban` TINYINT(1) UNSIGNED ZEROFILL DEFAULT NULL,
  `pin` VARCHAR(8) COLLATE latin1_swedish_ci NOT NULL,
  `ipAddress` VARCHAR(17) COLLATE latin1_swedish_ci DEFAULT NULL,
  `loginDate` DATETIME DEFAULT NULL,
  `logoutDate` DATETIME DEFAULT NULL,
  UNIQUE INDEX `account_id` USING BTREE (`id`) COMMENT ''
)ENGINE=InnoDB
AVG_ROW_LENGTH=8192 CHARACTER SET 'latin1' COLLATE 'latin1_swedish_ci'
COMMENT=''
;

#
# Structure for the `characters` table : 
#

CREATE TABLE `characters` (
  `created` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,
  `accountId` VARCHAR(11) COLLATE latin1_swedish_ci DEFAULT '',
  `name` VARCHAR(11) COLLATE latin1_swedish_ci NOT NULL DEFAULT '',
  `race` TINYINT(1) UNSIGNED DEFAULT 0,
  `level` SMALLINT(2) UNSIGNED NOT NULL DEFAULT 1,
  `levelUpPoints` SMALLINT(2) UNSIGNED DEFAULT 0,
  `experience` INTEGER(4) UNSIGNED DEFAULT 0,
  `strength` SMALLINT(2) UNSIGNED DEFAULT NULL,
  `agility` SMALLINT(2) UNSIGNED DEFAULT NULL,
  `vitality` SMALLINT(2) UNSIGNED DEFAULT NULL,
  `energy` SMALLINT(2) UNSIGNED DEFAULT NULL,
  `command` SMALLINT(2) UNSIGNED DEFAULT NULL,
  `money` INTEGER(2) UNSIGNED DEFAULT 0,
  `health` INTEGER(4) DEFAULT NULL,
  `maxHealth` INTEGER(4) DEFAULT NULL,
  `mana` INTEGER(4) DEFAULT NULL,
  `maxMana` INTEGER(4) DEFAULT NULL,
  `mapId` TINYINT(1) UNSIGNED DEFAULT NULL,
  `posX` TINYINT(1) UNSIGNED DEFAULT NULL,
  `posY` TINYINT(1) UNSIGNED DEFAULT NULL,
  `direction` TINYINT(1) DEFAULT NULL,
  `controlCode` TINYINT(1) UNSIGNED DEFAULT 0,
  UNIQUE INDEX `name` USING BTREE (`name`) COMMENT ''
)ENGINE=InnoDB
CHARACTER SET 'latin1' COLLATE 'latin1_swedish_ci'
COMMENT=''
;

#
# Definition for the `eMU_AccountCheck` function : 
#

DELIMITER $$

CREATE DEFINER = 'root'@'localhost' FUNCTION `eMU_AccountCheck`(
        `_id` VARCHAR(11),
        `_password` VARCHAR(11),
        `_ipAddress` VARCHAR(16)
    )
    RETURNS TINYINT(4)
    NOT DETERMINISTIC
    CONTAINS SQL
    SQL SECURITY DEFINER
    COMMENT ''
BEGIN
 DECLARE `exists_` TINYINT;
 DECLARE `passwordCheck_` TINYINT;
 DECLARE `status_` TINYINT;
 DECLARE `ban_` TINYINT;
 
 SELECT
 COUNT(*)
 INTO
 exists_ 
 FROM
 `accounts`
 WHERE
 `id` = `_id`;
 
 IF(exists_ > 0)
 THEN
 SELECT
 COUNT(*)
 INTO
 passwordCheck_
 FROM
 `accounts`
 WHERE
 `id` = `_id`
 AND
 `password` = `_password`;
 
 IF(passwordCheck_ > 0)
 THEN 
 SELECT
 `ban`
 INTO
 `ban_`
 FROM
 `accounts`
 WHERE
 `id` = `_id`;
 
 IF(`ban_` = 0)
 THEN
 SELECT
 `status`
 INTO
 `status_`
 FROM
 `accounts`
 WHERE
 `id` = `_id`;
 
 IF(`status_` = 0)
 THEN
 UPDATE
 `accounts`
 SET
 `status` = 1,
 `ipAddress` = _ipAddress,
 `loginDate` = NOW()
 WHERE
 `id` = _id;
 
 RETURN 0; -- ok.
 END IF;
 
 RETURN 3; -- in use.
 END IF;
 
 RETURN 5; -- banned.
 END IF;
 
 RETURN 2; -- invalid password.
 END IF;

 RETURN 1; -- not exists.
END$$

DELIMITER ;

#
# Definition for the `eMU_AccountCreate` function : 
#

DELIMITER $$

CREATE DEFINER = 'root'@'localhost' FUNCTION `eMU_AccountCreate`(
        `_id` VARCHAR(11),
        `_password` VARCHAR(11),
        `_pin` INTEGER(4)
    )
    RETURNS TINYINT(1)
    NOT DETERMINISTIC
    CONTAINS SQL
    SQL SECURITY DEFINER
    COMMENT ''
BEGIN
 DECLARE `exists_` TINYINT;
 
 SELECT
 COUNT(*)
 INTO
 `exists_`
 FROM
 `accounts`
 WHERE
 `id` = _id;
 
 IF(`exists_` = 0)
 THEN
 INSERT INTO
 `accounts`
 (`id`, `password`, `status`, `ban`, `pin`) VALUES(_id, _password, '0', '0', _pin);
 RETURN 0;
 END IF;
 
 RETURN 1;
END$$

DELIMITER ;

#
# Definition for the `eMU_CharacterCreate` function : 
#

DELIMITER $$

CREATE DEFINER = 'root'@'localhost' FUNCTION `eMU_CharacterCreate`(
        `_accountId` VARCHAR(11),
        `_name` VARCHAR(11),
        `_race` TINYINT
    )
    RETURNS TINYINT(4)
    NOT DETERMINISTIC
    CONTAINS SQL
    SQL SECURITY DEFINER
    COMMENT ''
BEGIN
 DECLARE count_ TINYINT;
 DECLARE exists_ TINYINT;
 DECLARE strength_ SMALLINT;
 DECLARE agility_ SMALLINT;
 DECLARE vitality_ SMALLINT;
 DECLARE energy_ SMALLINT;
 DECLARE command_ SMALLINT;
 DECLARE health_ FLOAT;
 DECLARE mana_ FLOAT;
 DECLARE mapId_ SMALLINT;
 DECLARE posX_ SMALLINT;
 DECLARE posY_ SMALLINT;
 
 SELECT
 COUNT(*)
 INTO
 `count_`
 FROM
 `characters`
 WHERE
 `accountId` = `_accountId`;
 
 SELECT
 COUNT(*)
 INTO
 `exists_`
 FROM
 `characters`
 WHERE 
 `name` = `_name`;
 
 IF(`exists_` = 0)
 THEN 
 SET `mapId_` = 0;
 SET `posX_` = 182;
 SET `posY_` = 128;
 SET `command_` = 0;
 
 CASE `_race`
 WHEN 0 THEN
 SET `strength_` = 18;
 SET `agility_` = 18;
 SET `vitality_` = 15;
 SET `energy_` = 30;
 SET `health_` = 60.0;
 SET `mana_` = 60.0;
 
 WHEN 16 THEN
 SET `strength_` = 28;
 SET `agility_` = 20;
 SET `vitality_` = 25;
 SET `energy_` = 10;
 SET `health_` = 110.0;
 SET `mana_` = 20.0;
 
 WHEN 32 THEN
 SET `strength_` = 22;
 SET `agility_` = 25;
 SET `vitality_` = 20;
 SET `energy_` = 15;
 SET `health_` = 80.0;
 SET `mana_` = 30.0;
 SET `mapId_` = 3;
 SET `posX_` = 175;
 SET `posY_` = 110;
 
 WHEN 48 THEN
 SET `strength_` = 26;
 SET `agility_` = 26;
 SET `vitality_` = 26;
 SET `energy_` = 26;
 SET `health_` = 110.0;
 SET `mana_` = 60.0;
 
 WHEN 64 THEN
 SET `strength_` = 26;
 SET `agility_` = 20;
 SET `vitality_` = 20;
 SET `energy_` = 15;
 SET `command_` = 25;
 SET `health_` = 90.0;
 SET `mana_` = 40.0;
 ELSE
 BEGIN
 END;
 END CASE;
 
 INSERT INTO
 `characters` (`accountId`, `name`, `race`,
 `strength`, `agility`, `vitality`,
 `energy`, `command`, `health`,
 `maxHealth`, `mana`, `maxMana`, 
 `mapId`, `posX`, `posY`) 
 VALUES
 (`_accountId`, `_name`, `_race`,
 `strength_`, `agility_`, `vitality_`,
 `energy_`, `command_`, `health_`,
 `health_`, `mana_`, `mana_`,
 `mapId_`, `posX_`, `posY_`); 
 RETURN 1;
 END IF;
 
 RETURN 0;
END$$

DELIMITER ;

#
# Definition for the `eMU_CharacterDelete` function : 
#

DELIMITER $$

CREATE DEFINER = 'root'@'localhost' FUNCTION `eMU_CharacterDelete`(
        `_accountId` VARCHAR(11),
        `_name` VARCHAR(11),
        `_pin` VARCHAR(8)
    )
    RETURNS TINYINT(4)
    NOT DETERMINISTIC
    CONTAINS SQL
    SQL SECURITY DEFINER
    COMMENT ''
BEGIN
 DECLARE exists_ TINYINT;
 DECLARE pinCheck_ TINYINT;
 
 SELECT
 COUNT(*)
 INTO
 `pinCheck_`
 FROM
 `accounts`
 WHERE
 `id` = `_accountId`
 AND
 `pin` = `_pin`;
 
 SELECT
 COUNT(*)
 INTO
 `exists_`
 FROM
 `characters`
 WHERE
 `name` = `_name`
 AND
 `accountId` = `_accountId`;
 
 IF(exists_ > 0)
 THEN
 IF(pinCheck_ > 0)
 THEN
 DELETE FROM
 `characters`
 WHERE
 name = `_name`;
 RETURN 1;
 END IF;
 
 RETURN 2;
 END IF;
 
 RETURN 0;
END$$

DELIMITER ;

#
# Data for the `accounts` table  (LIMIT -497,500)
#

INSERT INTO `accounts` (`id`, `password`, `status`, `ban`, `pin`, `ipAddress`, `loginDate`, `logoutDate`) VALUES

  ('acc','pwd',1,0,'1234','127.0.0.1','2013-12-11 13:44:51',NULL),
  ('loginek','haselko',1,0,'1234','127.0.0.1','2013-12-10 14:44:43',NULL);
COMMIT;



/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;