# Host: localhost  (Version: 5.6.13)
# Date: 2019-09-09 20:03:56
# Generator: MySQL-Front 5.3  (Build 4.271)

/*!40101 SET NAMES utf8 */;

#
# Structure for table "tbl_admin"
#

DROP TABLE IF EXISTS `tbl_admin`;
CREATE TABLE `tbl_admin` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `email` varchar(255) NOT NULL DEFAULT '',
  `password` varchar(255) NOT NULL DEFAULT '',
  `created` date DEFAULT NULL,
  `modified` date DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

#
# Data for table "tbl_admin"
#

INSERT INTO `tbl_admin` VALUES (1,'admin@admin.com','$2y$10$JIG72vnThCVxGMnKJVb3ButXa0tBuHeNw4fTn61tpZpFpDjC/.8aq','2017-09-30','2017-09-30');

#
# Structure for table "tbl_cond_as"
#

DROP TABLE IF EXISTS `tbl_cond_as`;
CREATE TABLE `tbl_cond_as` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned DEFAULT '0',
  `as_num` int(11) unsigned DEFAULT '0',
  `op` enum('SRC','DST') DEFAULT 'SRC',
  `descr` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_cond_as"
#

INSERT INTO `tbl_cond_as` VALUES (2,1,1234,'SRC',''),(3,1,12345,'DST','');

#
# Structure for table "tbl_cond_fwd_status"
#

DROP TABLE IF EXISTS `tbl_cond_fwd_status`;
CREATE TABLE `tbl_cond_fwd_status` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned NOT NULL DEFAULT '0',
  `status` int(2) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`),
  UNIQUE KEY `cond_id` (`cond_id`,`status`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

#
# Data for table "tbl_cond_fwd_status"
#


#
# Structure for table "tbl_cond_icmp"
#

DROP TABLE IF EXISTS `tbl_cond_icmp`;
CREATE TABLE `tbl_cond_icmp` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned NOT NULL DEFAULT '0',
  `type_id` int(11) unsigned NOT NULL DEFAULT '0',
  `code` int(2) unsigned NOT NULL DEFAULT '0',
  `descr` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`),
  UNIQUE KEY `cond_id` (`cond_id`,`type_id`,`code`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_cond_icmp"
#

INSERT INTO `tbl_cond_icmp` VALUES (2,2,1,6,'');

#
# Structure for table "tbl_cond_in_ifs"
#

DROP TABLE IF EXISTS `tbl_cond_in_ifs`;
CREATE TABLE `tbl_cond_in_ifs` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned NOT NULL DEFAULT '0',
  `if_num` int(2) unsigned DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Data for table "tbl_cond_in_ifs"
#


#
# Structure for table "tbl_cond_inout"
#

DROP TABLE IF EXISTS `tbl_cond_inout`;
CREATE TABLE `tbl_cond_inout` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned DEFAULT '0',
  `inout_num` int(11) unsigned DEFAULT '0',
  `op` enum('INPUT','OUTPUT') DEFAULT 'INPUT',
  `descr` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

#
# Data for table "tbl_cond_inout"
#

INSERT INTO `tbl_cond_inout` VALUES (2,1,5,'OUTPUT','');

#
# Structure for table "tbl_cond_ips"
#

DROP TABLE IF EXISTS `tbl_cond_ips`;
CREATE TABLE `tbl_cond_ips` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned NOT NULL DEFAULT '0',
  `ip` varchar(60) DEFAULT '',
  `op` enum('SRC','DST') DEFAULT 'SRC',
  `descr` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_cond_ips"
#

INSERT INTO `tbl_cond_ips` VALUES (4,1,'104.244.97.65','SRC',''),(5,1,'198.108.67.107','SRC','');

#
# Structure for table "tbl_cond_out_ifs"
#

DROP TABLE IF EXISTS `tbl_cond_out_ifs`;
CREATE TABLE `tbl_cond_out_ifs` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned NOT NULL DEFAULT '0',
  `if_num` int(2) unsigned DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

#
# Data for table "tbl_cond_out_ifs"
#


#
# Structure for table "tbl_cond_ports"
#

DROP TABLE IF EXISTS `tbl_cond_ports`;
CREATE TABLE `tbl_cond_ports` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned NOT NULL DEFAULT '0',
  `port` int(2) unsigned DEFAULT '0',
  `op` enum('SRC','DST') DEFAULT 'SRC',
  `descr` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_cond_ports"
#

INSERT INTO `tbl_cond_ports` VALUES (1,1,139,'SRC','samba'),(2,1,443,'SRC','samba1'),(4,1,1000,'SRC','');

#
# Structure for table "tbl_cond_protocol_versions"
#

DROP TABLE IF EXISTS `tbl_cond_protocol_versions`;
CREATE TABLE `tbl_cond_protocol_versions` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned NOT NULL DEFAULT '0',
  `version_id` int(11) unsigned DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_cond_protocol_versions"
#

INSERT INTO `tbl_cond_protocol_versions` VALUES (1,1,1),(2,1,2);

#
# Structure for table "tbl_cond_protocols"
#

DROP TABLE IF EXISTS `tbl_cond_protocols`;
CREATE TABLE `tbl_cond_protocols` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned NOT NULL DEFAULT '0',
  `protocol_id` int(11) unsigned DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_cond_protocols"
#

INSERT INTO `tbl_cond_protocols` VALUES (5,1,1),(6,1,2);

#
# Structure for table "tbl_cond_router"
#

DROP TABLE IF EXISTS `tbl_cond_router`;
CREATE TABLE `tbl_cond_router` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned DEFAULT '0',
  `router_id` int(11) unsigned DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Data for table "tbl_cond_router"
#


#
# Structure for table "tbl_cond_subnets"
#

DROP TABLE IF EXISTS `tbl_cond_subnets`;
CREATE TABLE `tbl_cond_subnets` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned NOT NULL DEFAULT '0',
  `ip` varchar(60) DEFAULT '',
  `mask` varchar(60) DEFAULT '16',
  `op` enum('SRC','DST') DEFAULT 'SRC',
  `descr` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_cond_subnets"
#

INSERT INTO `tbl_cond_subnets` VALUES (2,1,'125.0.0.1','16','SRC','net1 ');

#
# Structure for table "tbl_cond_tcp_flags"
#

DROP TABLE IF EXISTS `tbl_cond_tcp_flags`;
CREATE TABLE `tbl_cond_tcp_flags` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `cond_id` int(11) unsigned NOT NULL DEFAULT '0',
  `flag` int(2) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`),
  UNIQUE KEY `cond_id` (`cond_id`,`flag`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

#
# Data for table "tbl_cond_tcp_flags"
#

INSERT INTO `tbl_cond_tcp_flags` VALUES (4,1,1),(5,1,4),(6,1,16);

#
# Structure for table "tbl_condition"
#

DROP TABLE IF EXISTS `tbl_condition`;
CREATE TABLE `tbl_condition` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `descr` varchar(255) DEFAULT '',
  `ips_enable` enum('on','off') DEFAULT 'on',
  `ips_comp` enum('AND','OR') DEFAULT 'AND',
  `ports_enable` enum('on','off') DEFAULT 'on',
  `ports_comp` enum('AND','OR') DEFAULT 'AND',
  `routers_enable` enum('on','off') DEFAULT 'on',
  `routers_comp` enum('AND','OR') DEFAULT 'AND',
  `inout` enum('IN','OUT','ALL') DEFAULT 'ALL',
  `packets_enable` enum('on','off') DEFAULT 'on',
  `packets` int(3) unsigned DEFAULT '0',
  `packets_comp` varchar(60) DEFAULT 'EQ',
  `bytes_enable` enum('on','off') DEFAULT 'on',
  `bytes` int(3) unsigned DEFAULT '0',
  `bytes_comp` varchar(60) DEFAULT 'EQ',
  `bytes_scale` int(4) unsigned DEFAULT '0',
  `packets_ps_enable` enum('on','off') DEFAULT 'on',
  `packets_ps` int(3) unsigned DEFAULT '0',
  `packets_ps_comp` varchar(60) DEFAULT 'EQ',
  `packets_ps_scale` int(4) unsigned DEFAULT '0',
  `duration_enable` enum('on','off') DEFAULT 'on',
  `duration` int(3) unsigned DEFAULT '0',
  `duration_comp` varchar(60) DEFAULT 'EQ',
  `bps_enable` enum('on','off') DEFAULT 'on',
  `bps` int(3) unsigned DEFAULT '0',
  `bps_comp` varchar(60) DEFAULT 'EQ',
  `bps_scale` int(4) unsigned DEFAULT '0',
  `bpp_enable` enum('on','off') DEFAULT 'on',
  `bpp` int(3) unsigned DEFAULT '0',
  `bpp_comp` varchar(60) DEFAULT 'EQ',
  `bpp_scale` int(4) unsigned DEFAULT '0',
  `tos_enable` enum('on','off') DEFAULT 'on',
  `tos` int(3) unsigned DEFAULT '0',
  `as_enable` enum('on','off') DEFAULT 'on',
  `as_comp` enum('AND','OR') DEFAULT 'AND',
  `inouts_enable` enum('on','off') DEFAULT 'on',
  `inouts_comp` enum('AND','OR') DEFAULT 'AND',
  `subnets_enable` enum('on','off') DEFAULT 'on',
  `subnets_comp` enum('AND','OR') DEFAULT 'AND',
  `tcpflags_enable` enum('on','off') DEFAULT 'on',
  `tcpflags_comp` enum('AND','OR') DEFAULT 'AND',
  `fwdstatus_enable` enum('on','off') DEFAULT 'on',
  `fwdstatus_comp` enum('AND','OR') DEFAULT 'AND',
  `icmp_enable` enum('on','off') DEFAULT 'on',
  `icmp_comp` enum('AND','OR') DEFAULT 'AND',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_condition"
#

INSERT INTO `tbl_condition` VALUES (1,'Condition for NFCAPD 1','on','AND','on','AND','on','AND','ALL','on',10,'GT','on',1000,'GT',1024,'on',20,'GT',1024,'on',100,'GT','on',6000,'GT',1024,'on',1000,'GT',1024,'on',4,'on','AND','on','AND','on','AND','on','AND','on','AND','on','AND');

#
# Structure for table "tbl_fwd_status"
#

DROP TABLE IF EXISTS `tbl_fwd_status`;
CREATE TABLE `tbl_fwd_status` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `value` int(2) unsigned DEFAULT '0',
  `descr` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

#
# Data for table "tbl_fwd_status"
#

INSERT INTO `tbl_fwd_status` VALUES (1,0,'Unknown (Reason-code: Unknown)'),(2,64,'Forwarded (Reason-code: Unknown)'),(3,65,'Forwarded (Reason-code: Fragmented)'),(4,NULL,NULL);

#
# Structure for table "tbl_icmp_types"
#

DROP TABLE IF EXISTS `tbl_icmp_types`;
CREATE TABLE `tbl_icmp_types` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT '',
  `value` int(4) unsigned DEFAULT '0',
  `descr` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

#
# Data for table "tbl_icmp_types"
#

INSERT INTO `tbl_icmp_types` VALUES (1,'Echo Reply',0,'Echo Reply (Type 0)'),(2,'Destination Unreachable',3,'Destination Unreachable (Type 3)'),(3,'Source Quench ',4,'Source Quench (Deprecated Type 4)'),(4,'Redirect',5,'Redirect (Type 5)'),(5,'Alternate Host Address',6,'Alternate Host Address (Deprecated Type 6)');

#
# Structure for table "tbl_nfcapd"
#

DROP TABLE IF EXISTS `tbl_nfcapd`;
CREATE TABLE `tbl_nfcapd` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `descr` varchar(255) DEFAULT '',
  `cond_id` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_nfcapd"
#

INSERT INTO `tbl_nfcapd` VALUES (1,'NFCAPD_01',1),(2,'NFCAPD_02',1);

#
# Structure for table "tbl_protocol_versions"
#

DROP TABLE IF EXISTS `tbl_protocol_versions`;
CREATE TABLE `tbl_protocol_versions` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT '',
  `version` int(2) unsigned DEFAULT '0',
  `descr` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_protocol_versions"
#

INSERT INTO `tbl_protocol_versions` VALUES (1,'inet',0,'INET'),(2,'ipv4',0,'IPV4'),(3,'inet6',9,'descrlll'),(4,'ipv6',0,'IPV6');

#
# Structure for table "tbl_protocols"
#

DROP TABLE IF EXISTS `tbl_protocols`;
CREATE TABLE `tbl_protocols` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT '',
  `value` int(4) unsigned DEFAULT '0',
  `descr` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_protocols"
#

INSERT INTO `tbl_protocols` VALUES (1,'TCP',6,''),(2,'UDP',17,''),(3,'ICMP',0,'Internet control management protocol');

#
# Structure for table "tbl_router"
#

DROP TABLE IF EXISTS `tbl_router`;
CREATE TABLE `tbl_router` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(60) DEFAULT '',
  `descr` varchar(255) DEFAULT '',
  `detail` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_router"
#

INSERT INTO `tbl_router` VALUES (1,'170.170.1.1','main router','mac:XXX12 hhh'),(3,'10.10.1.1','2nd RT','36598');

#
# Structure for table "tbl_statics"
#

DROP TABLE IF EXISTS `tbl_statics`;
CREATE TABLE `tbl_statics` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `nfcapd_id` int(11) unsigned NOT NULL DEFAULT '0',
  `filter_id` int(11) unsigned DEFAULT '0',
  `type` int(4) unsigned DEFAULT '0',
  `size` int(4) unsigned DEFAULT '0',
  `flags` int(4) unsigned DEFAULT '0',
  `first` int(4) unsigned DEFAULT '0',
  `msec_first` int(4) unsigned DEFAULT '0',
  `last` int(4) unsigned DEFAULT '0',
  `msec_last` int(4) unsigned DEFAULT '0',
  `fwd_status` int(4) unsigned DEFAULT '0',
  `tcp_flags` int(4) unsigned DEFAULT '0',
  `prot` int(4) unsigned DEFAULT '0',
  `tos` int(4) unsigned DEFAULT '0',
  `srcport` int(4) unsigned DEFAULT '0',
  `dstport` int(4) unsigned DEFAULT '0',
  `srcaddr` varchar(60) DEFAULT '',
  `dstaddr` varchar(60) DEFAULT '',
  `dPkts` int(4) unsigned DEFAULT '0',
  `dOctets` int(4) unsigned DEFAULT '0',
  `out_pkts` int(4) unsigned DEFAULT '0',
  `out_bytes` int(4) unsigned DEFAULT '0',
  `ip_router` varchar(60) DEFAULT '',
  `srcas` int(4) unsigned DEFAULT '0',
  `dstas` int(4) unsigned DEFAULT '0',
  `input` int(4) unsigned DEFAULT '0',
  `output` int(4) unsigned DEFAULT '0',
  `received` int(8) unsigned DEFAULT '0',
  `update_dt` datetime DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Data for table "tbl_statics"
#


#
# Structure for table "tbl_tcp_flag"
#

DROP TABLE IF EXISTS `tbl_tcp_flag`;
CREATE TABLE `tbl_tcp_flag` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `value` int(2) unsigned DEFAULT '0',
  `descr` varchar(255) DEFAULT '',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;

#
# Data for table "tbl_tcp_flag"
#

INSERT INTO `tbl_tcp_flag` VALUES (2,0,'NULL'),(3,1,'FIN'),(4,2,'SYN'),(5,4,'RST'),(6,8,'PSH'),(7,16,'ACK'),(8,32,'URG'),(9,64,'ECE'),(10,128,'CWR'),(11,256,'NS');
