<?php declare(strict_types = 0);
/*
** Zabbix
** Copyright (C) 2001-2024 Zabbix SIA
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


namespace Widgets\ItemHistory\Includes;

use CButton,
	CCol,
	CColHeader,
	CDiv,
	CList,
	CTable,
	CTag,
	CVar;

use CWidgetFieldView;

class CWidgetFieldColumnsListView extends CWidgetFieldView {

	private const ZBX_STYLE_INACCESSIBLE = 'inaccessible';

	public function __construct(CWidgetFieldColumnsList $field) {
		$this->field = $field;
	}

	public function getView(): CTag {
		$columns = $this->field->getValue();

		$item_names = $columns ? $this->field->getItemNames(array_column($columns, 'itemid')) : [];

		$view = (new CTable())
			->setId('list_'.$this->field->getName())
			->setHeader([
				'',
				(new CColHeader(_('Name')))->addStyle('width: 39%'),
				(new CColHeader(_('Data')))->addStyle('width: 59%'),
				_('Action')
			]);

		foreach ($columns as $column_index => $column) {
			$column_data = [new CVar('sort_order['.$this->field->getName().'][]', $column_index)];

			foreach ($column as $key => $value) {
				$column_data[] = new CVar($this->field->getName().'['.$column_index.']['.$key.']', $value);
			}

			$inaccessible_item = !array_key_exists('itemid', $column)
				|| !array_key_exists($column['itemid'], $item_names);

			$item_name = !$inaccessible_item ? $item_names[$column['itemid']] : _('Inaccessible item');

			$view->addRow([
				(new CCol((new CDiv)->addClass(ZBX_STYLE_DRAG_ICON)))->addClass(ZBX_STYLE_TD_DRAG_ICON),
				(new CDiv($column['name']))
					->setTitle($column['name'])
					->addClass(ZBX_STYLE_OVERFLOW_ELLIPSIS),
				(new CDiv($item_name))
					->setTitle($item_name)
					->addClass(ZBX_STYLE_OVERFLOW_ELLIPSIS)
					->addClass($inaccessible_item ? self::ZBX_STYLE_INACCESSIBLE : null),
				(new CList([
					(new CButton('edit', _('Edit')))
						->addClass(ZBX_STYLE_BTN_LINK)
						->removeId(),
					(new CButton('remove', _('Remove')))
						->addClass(ZBX_STYLE_BTN_LINK)
						->removeId(),
					$column_data
				]))->addClass(ZBX_STYLE_HOR_LIST)
			]);
		}

		$view->addRow(
			(new CCol(
				(new CButton('add', _('Add')))
					->addClass(ZBX_STYLE_BTN_LINK)
					->setEnabled(!$this->isDisabled())
			))->setColSpan($view->getNumCols())
		);

		return $view;
	}
}
