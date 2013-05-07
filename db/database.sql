-- MySQL dump 10.13  Distrib 5.5.29, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: coordino_cit_hackaton
-- ------------------------------------------------------
-- Server version	5.5.29-0ubuntu0.12.04.1

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
-- Table structure for table `badges`
--

DROP TABLE IF EXISTS `badges`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `badges` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL,
  `description` text NOT NULL,
  `timestamp` int(12) NOT NULL,
  `user_id` int(12) NOT NULL,
  `image` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `badges`
--

LOCK TABLES `badges` WRITE;
/*!40000 ALTER TABLE `badges` DISABLE KEYS */;
/*!40000 ALTER TABLE `badges` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `bugs`
--

DROP TABLE IF EXISTS `bugs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bugs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `content` text NOT NULL,
  `status` enum('open','closed','invalid') NOT NULL DEFAULT 'open',
  `submitted` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `bugs`
--

LOCK TABLES `bugs` WRITE;
/*!40000 ALTER TABLE `bugs` DISABLE KEYS */;
/*!40000 ALTER TABLE `bugs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `comments`
--

DROP TABLE IF EXISTS `comments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `comments` (
  `id` int(255) NOT NULL AUTO_INCREMENT,
  `related_id` int(255) NOT NULL,
  `content` text NOT NULL,
  `timestamp` int(100) NOT NULL,
  `votes` smallint(5) NOT NULL,
  `user_id` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `comments`
--

LOCK TABLES `comments` WRITE;
/*!40000 ALTER TABLE `comments` DISABLE KEYS */;
/*!40000 ALTER TABLE `comments` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `histories`
--

DROP TABLE IF EXISTS `histories`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `histories` (
  `id` int(25) NOT NULL AUTO_INCREMENT,
  `type` varchar(255) NOT NULL,
  `related_id` int(25) NOT NULL,
  `user_id` int(25) NOT NULL,
  `timestamp` int(25) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `histories`
--

LOCK TABLES `histories` WRITE;
/*!40000 ALTER TABLE `histories` DISABLE KEYS */;
INSERT INTO `histories` VALUES (1,'asked',1,1,1250490231),(2,'asked',2,1,1367926171),(3,'edited',2,1,1367926198);
/*!40000 ALTER TABLE `histories` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `post_tags`
--

DROP TABLE IF EXISTS `post_tags`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `post_tags` (
  `post_id` int(255) NOT NULL,
  `tag_id` int(255) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `post_tags`
--

LOCK TABLES `post_tags` WRITE;
/*!40000 ALTER TABLE `post_tags` DISABLE KEYS */;
INSERT INTO `post_tags` VALUES (1,1),(1,2);
/*!40000 ALTER TABLE `post_tags` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `posts`
--

DROP TABLE IF EXISTS `posts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `posts` (
  `id` int(255) NOT NULL AUTO_INCREMENT,
  `type` enum('answer','question','approved','pending','spam') NOT NULL,
  `related_id` int(255) NOT NULL,
  `title` varchar(255) NOT NULL,
  `content` text NOT NULL,
  `status` enum('open','closed','correct') NOT NULL,
  `timestamp` int(100) NOT NULL,
  `last_edited_timestamp` int(100) NOT NULL,
  `user_id` int(10) NOT NULL,
  `votes` smallint(5) NOT NULL,
  `url_title` varchar(255) NOT NULL,
  `public_key` varchar(255) NOT NULL,
  `views` int(20) NOT NULL DEFAULT '1',
  `tags` text NOT NULL,
  `flags` smallint(3) NOT NULL,
  `notify` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`),
  FULLTEXT KEY `title` (`title`,`content`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `posts`
--

LOCK TABLES `posts` WRITE;
/*!40000 ALTER TABLE `posts` DISABLE KEYS */;
INSERT INTO `posts` VALUES (1,'question',0,'Test Coordino Question','<p>This is a sample <a href=\"http://www.coordino.com\">Coordino</a> question.</p>','open',1250490231,0,1,0,'test-coordino-question','4a88f7770778d',3,'',-2,1);
/*!40000 ALTER TABLE `posts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `posts_revs`
--

DROP TABLE IF EXISTS `posts_revs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `posts_revs` (
  `version_id` int(255) NOT NULL AUTO_INCREMENT,
  `version_created` datetime NOT NULL,
  `id` int(255) NOT NULL,
  `type` enum('answer','question') NOT NULL,
  `related_id` int(255) NOT NULL,
  `title` varchar(255) NOT NULL,
  `content` text NOT NULL,
  `status` enum('open','closed') NOT NULL,
  `timestamp` int(100) NOT NULL,
  `last_edited_timestamp` int(100) NOT NULL,
  `user_id` int(10) NOT NULL,
  `votes` smallint(5) NOT NULL,
  `url_title` varchar(255) NOT NULL,
  `public_key` varchar(255) NOT NULL,
  `views` int(20) NOT NULL DEFAULT '1',
  `tags` text NOT NULL,
  `flags` smallint(3) DEFAULT NULL,
  PRIMARY KEY (`version_id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `posts_revs`
--

LOCK TABLES `posts_revs` WRITE;
/*!40000 ALTER TABLE `posts_revs` DISABLE KEYS */;
INSERT INTO `posts_revs` VALUES (1,'2009-08-17 02:23:51',1,'question',0,'Test Coordino Question','<p>This is a sample <a href=\"http://www.coordino.com\">Coordino</a> question.</p>','open',1250490231,0,1,0,'test-coordino-question','4a88f7770778d',1,'sample, question',-2),(2,'2013-05-07 11:29:31',2,'question',0,'Teste Teste Teste','<p>Teste</p>','open',1367926171,0,1,0,'Teste-Teste-Teste','5188e59bb27e8',1,'',1),(3,'2013-05-07 11:29:58',2,'question',0,'Teste Teste Teste','<p>Teste</p>','open',1367926171,1367926198,1,0,'Teste-Teste-Teste','5188e59bb27e8',3,'teste-teste',1);
/*!40000 ALTER TABLE `posts_revs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `settings`
--

DROP TABLE IF EXISTS `settings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `settings` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `value` varchar(255) NOT NULL,
  `autoload` smallint(1) NOT NULL DEFAULT '0',
  `description` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=14 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `settings`
--

LOCK TABLES `settings` WRITE;
/*!40000 ALTER TABLE `settings` DISABLE KEYS */;
INSERT INTO `settings` VALUES (1,'rep_vote_up','0',1,'The amount of reputation required before a user can vote up a question or answer.'),(2,'rep_comment','0',1,'The amount of reputation required before a user can comment.'),(3,'rep_vote_down','0',1,'The amount of reputation required before a user can vote down a question or answer.'),(4,'rep_advertising','0',1,'The amount of reputation required before a user will no longer be show advertisements.'),(5,'rep_edit','1000',1,'The amount of reputation required before a user can edit another user\'s question or answer.'),(6,'rep_flag','0',0,'The amount of reputation required before a user can flag a question or answer.'),(7,'flag_display_limit','5',0,'The number of Flags a post needs to get before it is removed from public listings.'),(8,'remote_auth_only','no',0,'If set to \'yes\' logins will only be available via  third party login script.'),(9,'remote_auth_login_url','',0,'The URL that a user must login through to access the site.'),(10,'remote_auth_logout_url','',0,'The URL a user gets redirected to once they logout.'),(11,'site_maintenance','no',0,'If set to \'yes\', all pages should redirect to a message that the site is being updated/maintained.'),(12,'blacklist','0',1,'a:13:{i:0;s:4:\"fuck\";i:1;s:3:\"ass\";i:2;s:6:\"vagina\";i:3;s:4:\"cunt\";i:4;s:6:\"nigger\";i:5;s:3:\"nig\";i:6;s:5:\"penis\";i:7;s:5:\"dicks\";i:8;s:7:\"asshole\";i:9;s:8:\"assholes\";i:10;s:7:\"bitches\";i:11;s:5:\"bitch\";i:12;s:6:\"faggot\";}'),(13,'remote_auth_key','5188e43ab257e',1,'The authentication key allowing for remote logins.');
/*!40000 ALTER TABLE `settings` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tags`
--

DROP TABLE IF EXISTS `tags`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tags` (
  `id` int(150) NOT NULL AUTO_INCREMENT,
  `tag` varchar(255) NOT NULL,
  `slug` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tags`
--

LOCK TABLES `tags` WRITE;
/*!40000 ALTER TABLE `tags` DISABLE KEYS */;
INSERT INTO `tags` VALUES (1,'sample',''),(2,'question',''),(3,'teste-teste','');
/*!40000 ALTER TABLE `tags` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `username` varchar(60) NOT NULL,
  `url_title` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `joined` int(100) NOT NULL,
  `public_key` varchar(255) NOT NULL,
  `registered` smallint(1) NOT NULL,
  `reputation` int(10) NOT NULL,
  `website` varchar(255) NOT NULL,
  `location` varchar(255) NOT NULL,
  `age` smallint(3) NOT NULL,
  `info` text NOT NULL,
  `permission` text NOT NULL,
  `ip` varchar(20) NOT NULL,
  `answer_count` int(12) NOT NULL,
  `comment_count` int(12) NOT NULL,
  `question_count` int(12) NOT NULL,
  `image` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (1,'admin','admin','jribeiro@ciandt.com','e25aa9719199773c82483523d947d9af',1367925850,'5188e45a5235a',1,0,'','',0,'','a:5:{i:0;s:6:\"create\";i:1;s:4:\"read\";i:2;s:6:\"update\";i:3;s:6:\"delete\";i:4;s:5:\"admin\";}','127.0.0.1',0,0,2,'');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `votes`
--

DROP TABLE IF EXISTS `votes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `votes` (
  `id` int(255) NOT NULL AUTO_INCREMENT,
  `post_id` int(255) NOT NULL,
  `user_id` int(10) NOT NULL,
  `timestamp` int(100) NOT NULL,
  `type` enum('up','down','flag') NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `votes`
--

LOCK TABLES `votes` WRITE;
/*!40000 ALTER TABLE `votes` DISABLE KEYS */;
/*!40000 ALTER TABLE `votes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `widgets`
--

DROP TABLE IF EXISTS `widgets`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `widgets` (
  `id` smallint(5) NOT NULL AUTO_INCREMENT,
  `page` varchar(255) NOT NULL,
  `location` varchar(255) NOT NULL,
  `title` varchar(255) NOT NULL,
  `content` text NOT NULL,
  `global` smallint(1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `widgets`
--

LOCK TABLES `widgets` WRITE;
/*!40000 ALTER TABLE `widgets` DISABLE KEYS */;
/*!40000 ALTER TABLE `widgets` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-05-07  8:53:16
