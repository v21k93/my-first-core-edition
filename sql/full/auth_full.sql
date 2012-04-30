/*
SQLyog Community v9.63 
MySQL - 5.5.16-log : Database - auth
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`auth` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `auth`;

/*Table structure for table `account` */

DROP TABLE IF EXISTS `account`;

CREATE TABLE `account` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `username` varchar(32) NOT NULL DEFAULT '',
  `sha_pass_hash` varchar(40) NOT NULL DEFAULT '',
  `sessionkey` varchar(80) NOT NULL DEFAULT '',
  `v` varchar(64) NOT NULL DEFAULT '',
  `s` varchar(64) NOT NULL DEFAULT '',
  `email` varchar(254) NOT NULL DEFAULT '',
  `joindate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `last_ip` varchar(15) NOT NULL DEFAULT '127.0.0.1',
  `failed_logins` int(10) unsigned NOT NULL DEFAULT '0',
  `locked` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `last_login` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `online` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `expansion` tinyint(3) unsigned NOT NULL DEFAULT '2',
  `mutetime` bigint(20) NOT NULL DEFAULT '0',
  `locale` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `os` varchar(3) NOT NULL DEFAULT '',
  `recruiter` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COMMENT='Account System';

/*Data for the table `account` */

insert  into `account`(`id`,`username`,`sha_pass_hash`,`sessionkey`,`v`,`s`,`email`,`joindate`,`last_ip`,`failed_logins`,`locked`,`last_login`,`online`,`expansion`,`mutetime`,`locale`,`os`,`recruiter`) values (1,'ADMIN','8301316D0D8448A34FA6D0C6BF1CBFA2B4A1A93A','C93290124EEEAD3A2948510FFFBBBB029611C885C17FE433946A9780DD4515C5A16E36B3B0E58A88','718B0551BC118CF9EEDA0CF03177B5284885CADA1C6DD5D0345FD3C3D7E15A8E','A68DDCF6075E1A64982DAE7D4DD2F48A1566047452AA7757838DE0A4011F38D5','','2012-04-28 12:38:47','127.0.0.1',0,0,'2012-04-30 17:20:53',0,2,0,0,'Win',0);

/*Table structure for table `account_access` */

DROP TABLE IF EXISTS `account_access`;

CREATE TABLE `account_access` (
  `id` int(10) unsigned NOT NULL,
  `gmlevel` tinyint(3) unsigned NOT NULL,
  `RealmID` int(11) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`id`,`RealmID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `account_access` */

insert  into `account_access`(`id`,`gmlevel`,`RealmID`) values (1,5,-1);

/*Table structure for table `account_banned` */

DROP TABLE IF EXISTS `account_banned`;

CREATE TABLE `account_banned` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Account id',
  `bandate` int(10) unsigned NOT NULL DEFAULT '0',
  `unbandate` int(10) unsigned NOT NULL DEFAULT '0',
  `bannedby` varchar(50) NOT NULL,
  `banreason` varchar(255) NOT NULL,
  `active` tinyint(3) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`,`bandate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Ban List';

/*Data for the table `account_banned` */

/*Table structure for table `ip_banned` */

DROP TABLE IF EXISTS `ip_banned`;

CREATE TABLE `ip_banned` (
  `ip` varchar(15) NOT NULL DEFAULT '127.0.0.1',
  `bandate` int(10) unsigned NOT NULL,
  `unbandate` int(10) unsigned NOT NULL,
  `bannedby` varchar(50) NOT NULL DEFAULT '[Console]',
  `banreason` varchar(255) NOT NULL DEFAULT 'no reason',
  PRIMARY KEY (`ip`,`bandate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Banned IPs';

/*Data for the table `ip_banned` */

/*Table structure for table `logs` */

DROP TABLE IF EXISTS `logs`;

CREATE TABLE `logs` (
  `time` int(10) unsigned NOT NULL,
  `realm` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `string` text CHARACTER SET latin1
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `logs` */

/*Table structure for table `realmcharacters` */

DROP TABLE IF EXISTS `realmcharacters`;

CREATE TABLE `realmcharacters` (
  `realmid` int(10) unsigned NOT NULL DEFAULT '0',
  `acctid` int(10) unsigned NOT NULL,
  `numchars` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`realmid`,`acctid`),
  KEY `acctid` (`acctid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Realm Character Tracker';

/*Data for the table `realmcharacters` */

insert  into `realmcharacters`(`realmid`,`acctid`,`numchars`) values (1,2,2),(1,3,0);

/*Table structure for table `realmlist` */

DROP TABLE IF EXISTS `realmlist`;

CREATE TABLE `realmlist` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL DEFAULT '',
  `address` varchar(255) NOT NULL DEFAULT '127.0.0.1',
  `port` smallint(5) unsigned NOT NULL DEFAULT '8085',
  `icon` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `flag` tinyint(3) unsigned NOT NULL DEFAULT '2',
  `timezone` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `allowedSecurityLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `population` float unsigned NOT NULL DEFAULT '0',
  `gamebuild` int(10) unsigned NOT NULL DEFAULT '12340',
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Realm System';

/*Data for the table `realmlist` */

insert  into `realmlist`(`id`,`name`,`address`,`port`,`icon`,`flag`,`timezone`,`allowedSecurityLevel`,`population`,`gamebuild`) values (1,'Trinity','127.0.0.1',8085,0,2,1,0,0,12340);

/*Table structure for table `uptime` */

DROP TABLE IF EXISTS `uptime`;

CREATE TABLE `uptime` (
  `realmid` int(10) unsigned NOT NULL,
  `starttime` int(10) unsigned NOT NULL DEFAULT '0',
  `uptime` int(10) unsigned NOT NULL DEFAULT '0',
  `maxplayers` smallint(5) unsigned NOT NULL DEFAULT '0',
  `revision` varchar(255) NOT NULL DEFAULT 'Trinitycore',
  PRIMARY KEY (`realmid`,`starttime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Uptime system';

/*Data for the table `uptime` */

insert  into `uptime`(`realmid`,`starttime`,`uptime`,`maxplayers`,`revision`) values (1,1335605884,1201,1,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335607567,1824,1,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335615228,616,1,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335616102,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335616925,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335618268,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335679370,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335679772,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335680537,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335681007,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335683472,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335683998,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335686643,1801,1,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335691372,4203,1,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335695699,1202,1,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335703219,1801,2,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335722111,602,1,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335725852,602,2,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335727004,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335727835,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335763684,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335764723,602,2,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335776470,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335777105,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335778019,601,2,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335779031,601,2,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335780479,602,2,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335781636,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335783467,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335784279,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335787380,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335788156,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335791328,1201,2,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335793473,602,1,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335795619,1202,1,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)'),(1,1335801214,0,0,'TrinityCore rev. 0000-00-00 00:00:00 +0000 (Archived) (Win32, Release)');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
