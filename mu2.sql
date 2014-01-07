-- MySQL dump 10.13  Distrib 5.6.13, for Win32 (x86)
--
-- Host: localhost    Database: mu2
-- ------------------------------------------------------
-- Server version	5.5.34-0ubuntu0.13.10.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `mu2`
--

/*!40000 DROP DATABASE IF EXISTS `mu2`*/;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `mu2` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `mu2`;

--
-- Table structure for table `accounts`
--

DROP TABLE IF EXISTS `accounts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `accounts` (
  `id` varchar(11) NOT NULL DEFAULT '',
  `password` varchar(11) NOT NULL DEFAULT '',
  `status` tinyint(1) unsigned zerofill DEFAULT NULL,
  `ban` tinyint(1) unsigned zerofill DEFAULT NULL,
  `pin` varchar(8) NOT NULL,
  `ipAddress` varchar(17) DEFAULT NULL,
  `loginDate` datetime DEFAULT NULL,
  `logoutDate` datetime DEFAULT NULL,
  UNIQUE KEY `account_id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `accounts`
--

LOCK TABLES `accounts` WRITE;
/*!40000 ALTER TABLE `accounts` DISABLE KEYS */;
INSERT INTO `accounts` VALUES ('acc','pwd',0,0,'1234','127.0.0.1','2014-01-07 17:15:02',NULL),('loginek','haselko',0,0,'1234','127.0.0.1','2014-01-07 17:54:51',NULL);
/*!40000 ALTER TABLE `accounts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `characters`
--

DROP TABLE IF EXISTS `characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `characters` (
  `created` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `accountId` varchar(11) DEFAULT '',
  `name` varchar(11) NOT NULL DEFAULT '',
  `race` tinyint(1) unsigned DEFAULT '0',
  `level` smallint(2) unsigned NOT NULL DEFAULT '1',
  `levelUpPoints` smallint(2) unsigned DEFAULT '0',
  `experience` int(4) unsigned DEFAULT '0',
  `strength` smallint(2) unsigned DEFAULT NULL,
  `agility` smallint(2) unsigned DEFAULT NULL,
  `vitality` smallint(2) unsigned DEFAULT NULL,
  `energy` smallint(2) unsigned DEFAULT NULL,
  `command` smallint(2) unsigned DEFAULT NULL,
  `money` int(2) unsigned DEFAULT '0',
  `health` int(4) DEFAULT NULL,
  `maxHealth` int(4) DEFAULT NULL,
  `mana` int(4) DEFAULT NULL,
  `maxMana` int(4) DEFAULT NULL,
  `mapId` tinyint(1) unsigned DEFAULT NULL,
  `posX` tinyint(1) unsigned DEFAULT NULL,
  `posY` tinyint(1) unsigned DEFAULT NULL,
  `direction` tinyint(1) DEFAULT NULL,
  `controlCode` tinyint(1) unsigned DEFAULT '0',
  `hairColor` smallint(6) DEFAULT NULL,
  `hairType` smallint(6) DEFAULT NULL,
  `tutorialState` smallint(6) DEFAULT NULL,
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `characters`
--

LOCK TABLES `characters` WRITE;
/*!40000 ALTER TABLE `characters` DISABLE KEYS */;
INSERT INTO `characters` VALUES ('2014-01-06 11:00:00','loginek','andrzej',1,43,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1),('2014-01-06 11:00:00','acc','grzesiu',1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1),('2014-01-06 11:00:00','loginek','krzysiu',2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1),('2014-01-06 11:00:00','loginek','roman',4,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1),('2014-01-06 11:00:00','loginek','waldek',8,120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1);
/*!40000 ALTER TABLE `characters` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping events for database 'mu2'
--

--
-- Dumping routines for database 'mu2'
--
/*!50003 DROP FUNCTION IF EXISTS `eMU_AccountCheck` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `eMU_AccountCheck`(
        `_id` VARCHAR(11),
        `_password` VARCHAR(11),
        `_ipAddress` VARCHAR(16)
    ) RETURNS tinyint(4)
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
 `status` = 0,
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
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `eMU_AccountCreate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `eMU_AccountCreate`(
 _id VARCHAR(11),
 _password VARCHAR(11),
 _pin INTEGER(4)
 ) RETURNS tinyint(1)
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
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `eMU_CharacterCreate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `eMU_CharacterCreate`(
 _accountId VARCHAR(11),
 _name VARCHAR(11),
 _race TINYINT
 ) RETURNS tinyint(4)
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
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `eMU_CharacterDelete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `eMU_CharacterDelete`(
 _accountId VARCHAR(11),
 _name VARCHAR(11),
 _pin VARCHAR(8)
 ) RETURNS tinyint(4)
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
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-01-07 18:11:34
