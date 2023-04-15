-- phpMyAdmin SQL Dump
-- version 4.6.6deb5
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Apr 15, 2023 at 08:54 PM
-- Server version: 10.3.38-MariaDB-0+deb10u1
-- PHP Version: 7.3.31-1~deb10u3

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `plant_database`
--

-- --------------------------------------------------------

--
-- Table structure for table `humidity`
--

CREATE TABLE `humidity` (
  `measurement_datetime` datetime NOT NULL,
  `units` decimal(10,0) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `light`
--

CREATE TABLE `light` (
  `id` int(11) NOT NULL,
  `turn_on` time DEFAULT NULL,
  `turn_off` time DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `moisture`
--

CREATE TABLE `moisture` (
  `sensor_id` int(10) NOT NULL,
  `measurement_datetime` datetime NOT NULL,
  `units` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `moisture_sensor`
--

CREATE TABLE `moisture_sensor` (
  `plant_digital_output_pin` int(11) NOT NULL,
  `sensor_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `plant`
--

CREATE TABLE `plant` (
  `digital_output_pin` int(10) NOT NULL,
  `name` text NOT NULL,
  `hours_between_waterings` double DEFAULT NULL,
  `last_watered` datetime NOT NULL DEFAULT current_timestamp(),
  `water_volume` double DEFAULT NULL,
  `light_id` int(2) DEFAULT NULL,
  `target_moisture` int(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `temperature`
--

CREATE TABLE `temperature` (
  `measurement_datetime` datetime NOT NULL,
  `units` decimal(10,0) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `humidity`
--
ALTER TABLE `humidity`
  ADD PRIMARY KEY (`measurement_datetime`);

--
-- Indexes for table `light`
--
ALTER TABLE `light`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `moisture`
--
ALTER TABLE `moisture`
  ADD PRIMARY KEY (`sensor_id`,`measurement_datetime`),
  ADD KEY `plant_digital_output_pin` (`sensor_id`);

--
-- Indexes for table `moisture_sensor`
--
ALTER TABLE `moisture_sensor`
  ADD PRIMARY KEY (`sensor_id`),
  ADD KEY `plant_digital_output_pin` (`plant_digital_output_pin`);

--
-- Indexes for table `plant`
--
ALTER TABLE `plant`
  ADD PRIMARY KEY (`digital_output_pin`),
  ADD KEY `foreign_key` (`light_id`);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `moisture`
--
ALTER TABLE `moisture`
  ADD CONSTRAINT `moisture_ibfk_1` FOREIGN KEY (`sensor_id`) REFERENCES `moisture_sensor` (`sensor_id`) ON DELETE CASCADE;

--
-- Constraints for table `moisture_sensor`
--
ALTER TABLE `moisture_sensor`
  ADD CONSTRAINT `moisture_sensor_ibfk_1` FOREIGN KEY (`plant_digital_output_pin`) REFERENCES `plant` (`digital_output_pin`);

--
-- Constraints for table `plant`
--
ALTER TABLE `plant`
  ADD CONSTRAINT `foreign_key` FOREIGN KEY (`light_id`) REFERENCES `light` (`id`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
