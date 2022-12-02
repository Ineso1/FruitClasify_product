CREATE TABLE `maquina` (
  `id_Maquina` int(11) NOT NULL,
  `ip_Maquina` int(20) NOT NULL,
  `direccion` varchar(100) NOT NULL,
  `empresa` varchar(30) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
ALTER TABLE `maquina`
  ADD PRIMARY KEY (`id_Maquina`);
ALTER TABLE `maquina`
  MODIFY `id_Maquina` int(11) NOT NULL AUTO_INCREMENT;

CREATE TABLE `muestra` (
    `id_Muestra` int(10) NOT NULL,
    `id_Maquina` int(11) NOT NULL,
    `peso` int(11) NOT NULL,
    `diametro` int(11) NOT NULL,
    `calidad` varchar(11) NOT NULL,
    `fecha` date NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
ALTER TABLE `muestra`
    ADD PRIMARY KEY (`id_Muestra`);
ALTER TABLE `muestra`
  MODIFY `id_Muestra` int(10) NOT NULL AUTO_INCREMENT;
COMMIT;
