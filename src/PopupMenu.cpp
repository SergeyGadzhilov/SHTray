/*
 * SHTray
 * Copyright (C) 2022  Hadzhilov Serhii
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "PopupMenu.h"

PopupMenu::PopupMenu(HWND hWnd) :
    m_parent(hWnd)
{
    m_menu = CreatePopupMenu();
}

PopupMenu::~PopupMenu()
{
    DestroyMenu(m_menu);
}

void PopupMenu::show()
{
    SetFocus(m_parent);
    SetMenuDefaultItem(m_menu, 0, FALSE);
    SendMessage(m_parent, WM_INITMENUPOPUP, (WPARAM)m_menu, 0);

    POINT position = {0};
    GetCursorPos(&position);

    auto flags = TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY;
    WORD cmd = TrackPopupMenu(m_menu, flags, position.x, position.y, 0, m_parent, NULL);
    SendMessage(m_parent, WM_COMMAND, cmd, 0);
}

void PopupMenu::addItem(const std::string &name)
{
    const auto id = GetMenuItemCount(m_menu);
    InsertMenu(m_menu, id, MF_BYPOSITION|MF_STRING, id, name.c_str());
}
