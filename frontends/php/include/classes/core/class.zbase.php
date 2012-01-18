<?php
/*
** Zabbix
** Copyright (C) 2000-2012 Zabbix SIA
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/
?>
<?php

require_once dirname(__FILE__).'/class.czabbixautoloader.php';

class ZBase {

	/**
	 * An instance of the current Z object.
	 *
	 * @var Z
	 */
	protected static $instance;

	/**
	 * The absolute path to the root directory.
	 *
	 * @var string
	 */
	private $rootDir;

	/**
	 * Returns the current instance of Z.
	 *
	 * @static
	 *
	 * @return Z
	 */
	public static function i() {
		if (self::$instance === null) {
			self::$instance = new Z();
		}

		return self::$instance;
	}

	/**
	 * Initializes the application.
	 */
	public function run() {
		$this->setRootDir();
		$this->registerAutoloader();
	}

	/**
	 * Returns the absolute path to the root dir.
	 *
	 * @return string
	 */
	public function getRootDir() {
		return $this->rootDir;
	}

	/**
	 * Set root dir absolute path.
	 */
	private function setRootDir() {
		$this->rootDir = realpath(dirname(__FILE__).DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'..');
	}

	/**
	 * Register autoloader.
	 */
	private function registerAutoloader() {
		$autoloader = new CZabbixAutoloader($this->getIncludePaths());
		$autoloader->register();
	}

	/**
	 * An array of directories to add to the autoloader include paths.
	 *
	 * @return array
	 */
	private function getIncludePaths() {
		return array(
			self::$instance->rootDir.DIRECTORY_SEPARATOR.'include'.DIRECTORY_SEPARATOR.
					'classes'.DIRECTORY_SEPARATOR,
			self::$instance->rootDir.DIRECTORY_SEPARATOR.'include'.DIRECTORY_SEPARATOR.
					'classes'.DIRECTORY_SEPARATOR.'sysmaps'.DIRECTORY_SEPARATOR,
			self::$instance->rootDir.DIRECTORY_SEPARATOR.'api'.DIRECTORY_SEPARATOR.
					'classes'.DIRECTORY_SEPARATOR,
			self::$instance->rootDir.DIRECTORY_SEPARATOR.'api'.DIRECTORY_SEPARATOR.
					'rpc'.DIRECTORY_SEPARATOR
		);
	}
}
