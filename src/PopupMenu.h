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
#ifndef POPUPMENU_H
#define POPUPMENU_H

#include <string>
#include <windows.h>

class PopupMenu
{
public:
    explicit PopupMenu(HWND hWnd);
    ~PopupMenu();

    void show();
    void addItem(const std::string& name);

private:
    HWND m_parent = nullptr;
    HMENU m_menu = nullptr;
};

#endif // POPUPMENU_H
