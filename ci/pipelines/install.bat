rem Copyright (C) 2022 dmiron
rem
rem This file is part of chemicalfun.
rem
rem chemicalfun is free software: you can redistribute it and/or modify
rem it under the terms of the GNU Lesser General Public License as published by
rem the Free Software Foundation, either version 2.1 of the License, or
rem (at your option) any later version.
rem
rem chemicalfun is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem GNU Lesser General Public License for more details.
rem
rem You should have received a copy of the GNU Lesser General Public License
rem along with chemicalfun.  If not, see <http://www.gnu.org/licenses/>.

mkdir build
cd build

call activate chemicalfun

cmake -G Ninja ^
    -DCHEMICALFUN_PYTHON_INSTALL_PREFIX:PATH="%CONDA_PREFIX%" ^
    -DCMAKE_BUILD_TYPE=Release ^
    ..
ninja install
