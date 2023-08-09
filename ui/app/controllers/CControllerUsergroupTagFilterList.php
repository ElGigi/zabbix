<?php declare(strict_types = 0);
/*
** Zabbix
** Copyright (C) 2001-2023 Zabbix SIA
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

class CControllerUsergroupTagFilterList extends CController {

	protected function init(): void {
		$this->setPostContentType(self::POST_CONTENT_TYPE_JSON);
		$this->disableCsrfValidation();
	}

	protected function checkInput(): bool {
		$fields = [
			'tag_filters' => 'array'
		];

		$ret = $this->validateInput($fields);

		if (!$ret) {
			$this->setResponse(
				(new CControllerResponseData(['main_block' => json_encode([
					'error' => [
						'messages' => array_column(get_and_clear_messages(), 'message')
					]
				])]))->disableView()
			);
		}

		return $ret;
	}

	protected function checkPermissions(): bool {
		return $this->checkAccess(CRoleHelper::UI_ADMINISTRATION_USER_GROUPS);
	}

	protected function doAction(): void {
		$data = [
			'tag_filters' => $this->getInput('tag_filters', [])
		];

		$html_tag_filters = $data['tag_filters'];

		foreach ($html_tag_filters as &$group) {
			if (empty($group['tags']) || (count($group['tags']) === 1 && $group['tags'][0]['tag'] === '')) {
				$group['tags'] = [
					[
						'tag' => 'All tags',
						'value' => ''
					]
				];
			}
		}
		unset($group);

		$data['html_tag_filters'] = makeTags($html_tag_filters, true, 'groupid');

		$this->setResponse(new CControllerResponseData($data));
	}
}
