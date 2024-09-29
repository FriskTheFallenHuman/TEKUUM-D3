CODE LICENSE EXCEPTIONS - The parts that are not covered by the GPL:

EXCLUDED CODE:  The code described below and contained in the Doom 3 BFG Edition GPL Source Code release
is not part of the Program covered by the GPL and is expressly excluded from its terms. 
You are solely responsible for obtaining from the copyright holder a license for such code and complying with the applicable license terms.


Base64 implementation
---------------------------------------------------------------------------
neo/idlib/Base64.cpp

Copyright (c) 1996 Lars Wirzenius.  All rights reserved.

June 14 2003: TTimo <ttimo@idsoftware.com>
	modified + endian bug fixes
	http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=197039

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

miniz library
-----------------------------------------------------------------------------
neo/libs/miniz/*

Copyright 2013-2014 RAD Game Tools and Valve Software
Copyright 2010-2014 Rich Geldreich and Tenacious Software LLC

All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

IO for (un)compress .zip files using zlib
---------------------------------------------------------------------------
neo/libs/zlib/minizip/*

NOTE from Daniel Gibson:

These are parts of MiniZip (the unzip-parts), taken from zlib's contrib/minizip/,
the now-current development branch (commit f56ad0aafa868303ce324ae8d45675570c66b050
"Note termination of returned strings in contrib/minizip/unzip.h." from 2024-01-23)
and adjusted for doom3 by making small adjustments for doom3 (doom3-specific allocator,
which included reintroducing the TRYFREE macro that was removed upstream)
and adding a function (unzReOpen()) from the original Doom3 GPL release by id Software 
that used MiniZip 0.15beta. It was adjusted by Daniel Gibson for MiniZip 1.1
 Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
 Copyright (C) 2012,2024 Daniel Gibson

########### Original Content of this File below #############

Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

Modifications of Unzip for Zip64
Copyright (C) 2007-2008 Even Rouault

Modifications for Zip64 support
Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

MD4 Message-Digest Algorithm
-----------------------------------------------------------------------------
neo/idlib/hashing/MD4.cpp
Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD4 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD4 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

MD5 Message-Digest Algorithm
-----------------------------------------------------------------------------
neo/idlib/hashing/MD5.cpp
This code implements the MD5 message-digest algorithm.
The algorithm is due to Ron Rivest.  This code was
written by Colin Plumb in 1993, no copyright is claimed.
This code is in the public domain; do with it what you wish.

CRC32 Checksum
-----------------------------------------------------------------------------
neo/idlib/hashing/CRC32.cpp
Copyright (C) 1995-1998 Mark Adler

Renderdoc API
---------------------------------------------------------------------------
neo/libs/renderdoc/

The MIT License (MIT)

Copyright (c) 2019-2022 Baldur Karlsson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

The OpenGL Extension Wrangler Library (GLEW)
---------------------------------------------------------------------------
Copyright (C) 2002-2007, Milan Ikits <milan ikits[]ieee org>
Copyright (C) 2002-2007, Marcelo E. Magallon <mmagallo[]debian org>
Copyright (C) 2002, Lev Povalahev
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* The name of the author may be used to endorse or promote products 
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.


Mesa 3-D graphics library
Version:  7.0

Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


Copyright (c) 2007 The Khronos Group Inc.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and/or associated documentation files (the
"Materials"), to deal in the Materials without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Materials, and to
permit persons to whom the Materials are furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Materials.

THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.

libbinkdec
---------------------------------------------------------------------------
neo/libs/libbinkdec/*

Copyright (C) 2011 Barry Duncan

Based on Bink video decoder from FFmpeg

 Copyright (C) 2009 Konstantin Shishkov
 Copyright (C) 2011 Peter Ross <pross@xvid.org>

And generic Code from FFmpeg

 Copyright (C) 2000-2011 FFmpeg team, http://www.ffmpeg.org

libbinkdec and (the used parts of) FFmpeg are released under LGPL v2.1:

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

(You can find the whole license text on https://www.gnu.org/licenses/lgpl-2.1.html
 or in neo/libs/libbinkdec/COPYING)

ImGui
---------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2014-2019 Omar Cornut

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

TinyEXR
---------------------------------------------------------------------------
neo/libs/tinyexr/*

Copyright (c) 2014 - 2020, Syoyo Fujita and many contributors.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Syoyo Fujita nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


TinyEXR contains some OpenEXR code, which is licensed under

OpenEXR
---------------------------------------------------------------------------
Copyright (c) 2002, Industrial Light & Magic, a division of Lucas
Digital Ltd. LLC

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
*       Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
*       Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.
*       Neither the name of Industrial Light & Magic nor the names of
its contributors may be used to endorse or promote products derived
from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Mesa R11G11B10 utility functions
-----------------------------------------------------------------------------
neo/libs/mesa/format_r11g11b10f.h

/*
 * Copyright (C) 2011 Marek Olšák <maraeo@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/* Based on code from The OpenGL Programming Guide / 7th Edition, Appendix J.
 * Available here: http://www.opengl-redbook.com/appendices/
 * The algorithm in the book contains a bug though, which is fixed in the code
 * below.
 */


Mikkelsen Tangent Space library
-----------------------------------------------------------------------------
neo/libs/mikktspace/*

Copyright (C) 2011 by Morten S. Mikkelsen

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.


STB single header libraries
-----------------------------------------------------------------------------
neo/libs/stb/*

These libraries are in the public domain. You can do anything you want with them.
You have no legal obligation to do anything else, although I appreciate attribution.

They are also licensed under the MIT open source license, if you have lawyers who are unhappy with public domain. 
Every source file includes an explicit dual-license for you to choose from.


 BinPack2D is a 2 dimensional, multi-bin, bin-packer.
-----------------------------------------------------------------------------
neo/libs/binpack2d/*

Copyright (c) 2013, Christopher Stones < chris.stones@zoho.com >
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


Research Samples by Morgan McGuire
-----------------------------------------------------------------------------
base/renderprogs/builtin/SSAO/*   (AlchemyAO)
base/renderprogs/builtin/SSGI/*   (Deep G-Buffer)

Morgan McGuire and Michael Mara, NVIDIA Research

Open Source under the "BSD" license: http://www.opensource.org/licenses/bsd-license.php

Copyright (c) 2011-2014, NVIDIA
Copyright (c) 2016 Robert Beckebans ( id Tech 4.x integration )
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Optick: C++ Profiler For Games
-----------------------------------------------------------------------------
neo/libs/optick

The MIT License (MIT)

Copyright (c) 2019 Vadim Slyusarev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


AMD Vulkan Memory Allocator
-----------------------------------------------------------------------------
neo/libs/vma/

Copyright (c) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Intel Masked Occlusion Culling 
-----------------------------------------------------------------------------
neo/libs/moc/

Apache License
 Version 2.0, January 2004

 http://www.apache.org/licenses/ 

TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION 

1. Definitions.

"License" shall mean the terms and conditions for use, reproduction, and 
distribution as defined by Sections 1 through 9 of this document. 

"Licensor" shall mean the copyright owner or entity authorized by the copyright 
owner that is granting the License. 

"Legal Entity" shall mean the union of the acting entity and all other entities 
that control, are controlled by, or are under common control with that entity. 
For the purposes of this definition, "control" means (i) the power, direct or 
indirect, to cause the direction or management of such entity, whether by 
contract or otherwise, or (ii) ownership of fifty percent (50%) or more of the 
outstanding shares, or (iii) beneficial ownership of such entity. 

"You" (or "Your") shall mean an individual or Legal Entity exercising 
permissions granted by this License. 

"Source" form shall mean the preferred form for making modifications, including 
but not limited to software source code, documentation source, and configuration 
files. 

"Object" form shall mean any form resulting from mechanical transformation or 
translation of a Source form, including but not limited to compiled object code, 
generated documentation, and conversions to other media types. 

"Work" shall mean the work of authorship, whether in Source or Object form, made 
available under the License, as indicated by a copyright notice that is included 
in or attached to the work (an example is provided in the Appendix below). 

"Derivative Works" shall mean any work, whether in Source or Object form, that 
is based on (or derived from) the Work and for which the editorial revisions, 
annotations, elaborations, or other modifications represent, as a whole, an 
original work of authorship. For the purposes of this License, Derivative Works 
shall not include works that remain separable from, or merely link (or bind by 
name) to the interfaces of, the Work and Derivative Works thereof. 

"Contribution" shall mean any work of authorship, including the original version 
of the Work and any modifications or additions to that Work or Derivative Works 
thereof, that is intentionally submitted to Licensor for inclusion in the Work 
by the copyright owner or by an individual or Legal Entity authorized to submit 
on behalf of the copyright owner. For the purposes of this definition, 
"submitted" means any form of electronic, verbal, or written communication sent 
to the Licensor or its representatives, including but not limited to 
communication on electronic mailing lists, source code control systems, and 
issue tracking systems that are managed by, or on behalf of, the Licensor for 
the purpose of discussing and improving the Work, but excluding communication 
that is conspicuously marked or otherwise designated in writing by the copyright 
owner as "Not a Contribution." 

"Contributor" shall mean Licensor and any individual or Legal Entity on behalf 
of whom a Contribution has been received by Licensor and subsequently 
incorporated within the Work. 

2. Grant of Copyright License. Subject to the terms and conditions of this 
License, each Contributor hereby grants to You a perpetual, worldwide, 
non-exclusive, no-charge, royalty-free, irrevocable copyright license to 
reproduce, prepare Derivative Works of, publicly display, publicly perform, 
sublicense, and distribute the Work and such Derivative Works in Source or 
Object form. 

3. Grant of Patent License. Subject to the terms and conditions of this License, 
each Contributor hereby grants to You a perpetual, worldwide, non-exclusive, 
no-charge, royalty-free, irrevocable (except as stated in this section) patent 
license to make, have made, use, offer to sell, sell, import, and otherwise 
transfer the Work, where such license applies only to those patent claims 
licensable by such Contributor that are necessarily infringed by their 
Contribution(s) alone or by combination of their Contribution(s) with the Work 
to which such Contribution(s) was submitted. If You institute patent litigation 
against any entity (including a cross-claim or counterclaim in a lawsuit) 
alleging that the Work or a Contribution incorporated within the Work 
constitutes direct or contributory patent infringement, then any patent licenses 
granted to You under this License for that Work shall terminate as of the date 
such litigation is filed. 

4. Redistribution. You may reproduce and distribute copies of the Work or 
Derivative Works thereof in any medium, with or without modifications, and in 
Source or Object form, provided that You meet the following conditions: 
  You must give any other recipients of the Work or Derivative Works a copy of 
  this License; and 


  You must cause any modified files to carry prominent notices stating that You 
  changed the files; and 


  You must retain, in the Source form of any Derivative Works that You 
  distribute, all copyright, patent, trademark, and attribution notices from the 
  Source form of the Work, excluding those notices that do not pertain to any 
  part of the Derivative Works; and 


  If the Work includes a "NOTICE" text file as part of its distribution, then 
  any Derivative Works that You distribute must include a readable copy of the 
  attribution notices contained within such NOTICE file, excluding those notices 
  that do not pertain to any part of the Derivative Works, in at least one of 
  the following places: within a NOTICE text file distributed as part of the 
  Derivative Works; within the Source form or documentation, if provided along 
  with the Derivative Works; or, within a display generated by the Derivative 
  Works, if and wherever such third-party notices normally appear. The contents 
  of the NOTICE file are for informational purposes only and do not modify the 
  License. You may add Your own attribution notices within Derivative Works that 
  You distribute, alongside or as an addendum to the NOTICE text from the Work, 
  provided that such additional attribution notices cannot be construed as 
  modifying the License.
You may add Your own copyright statement to Your modifications and may provide 
additional or different license terms and conditions for use, reproduction, or 
distribution of Your modifications, or for any such Derivative Works as a whole, 
provided Your use, reproduction, and distribution of the Work otherwise complies 
with the conditions stated in this License. 

5. Submission of Contributions. Unless You explicitly state otherwise, any 
Contribution intentionally submitted for inclusion in the Work by You to the 
Licensor shall be under the terms and conditions of this License, without any 
additional terms or conditions. Notwithstanding the above, nothing herein shall 
supersede or modify the terms of any separate license agreement you may have 
executed with Licensor regarding such Contributions. 

6. Trademarks. This License does not grant permission to use the trade names, 
trademarks, service marks, or product names of the Licensor, except as required 
for reasonable and customary use in describing the origin of the Work and 
reproducing the content of the NOTICE file. 

7. Disclaimer of Warranty. Unless required by applicable law or agreed to in 
writing, Licensor provides the Work (and each Contributor provides its 
Contributions) on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY 
KIND, either express or implied, including, without limitation, any warranties 
or conditions of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A 
PARTICULAR PURPOSE. You are solely responsible for determining the 
appropriateness of using or redistributing the Work and assume any risks 
associated with Your exercise of permissions under this License. 

8. Limitation of Liability. In no event and under no legal theory, whether in 
tort (including negligence), contract, or otherwise, unless required by 
applicable law (such as deliberate and grossly negligent acts) or agreed to in 
writing, shall any Contributor be liable to You for damages, including any 
direct, indirect, special, incidental, or consequential damages of any character 
arising as a result of this License or out of the use or inability to use the 
Work (including but not limited to damages for loss of goodwill, work stoppage, 
computer failure or malfunction, or any and all other commercial damages or 
losses), even if such Contributor has been advised of the possibility of such 
damages. 

9. Accepting Warranty or Additional Liability. While redistributing the Work or 
Derivative Works thereof, You may choose to offer, and charge a fee for, 
acceptance of support, warranty, indemnity, or other liability obligations 
and/or rights consistent with this License. However, in accepting such 
obligations, You may act only on Your own behalf and on Your sole 
responsibility, not on behalf of any other Contributor, and only if You agree to 
indemnify, defend, and hold each Contributor harmless for any liability incurred 
by, or claims asserted against, such Contributor by reason of your accepting any 
such warranty or additional liability. 

END OF TERMS AND CONDITIONS 

APPENDIX: How to apply the Apache License to your work 

To apply the Apache License to your work, attach the following boilerplate 
notice, with the fields enclosed by brackets "[]" replaced with your own 
identifying information. (Don't include the brackets!) The text should be 
enclosed in the appropriate comment syntax for the file format. We also 
recommend that a file or class name and description of purpose be included on 
the same "printed page" as the copyright notice for easier identification within 
third-party archives. 

Copyright [yyyy] [name of copyright owner] Licensed under the Apache License, 
Version 2.0 (the "License"); you may not use this file except in compliance with 
the License. You may obtain a copy of the License at 
http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or 
agreed to in writing, software distributed under the License is distributed on 
an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express 
or implied. See the License for the specific language governing permissions and 
limitations under the License.


Editor replacement textures that were missing in the BFG release
-----------------------------------------------------------------------------
textures/common/*
textures/editor/*

Copyright (c) 2013 Robert Beckebans
Copyright (c) 2019 Maximiliano Ruben Viamonte aka 'Stradex'

CC0 1.0 Universal (CC0 1.0)
Public Domain Dedication

No Copyright

The person who associated a work with this deed has dedicated the work to the public domain by waiving all of his or her rights to the work worldwide under copyright law, including all related and neighboring rights, to the extent allowed by law.

You can copy, modify, distribute and perform the work, even for commercial purposes, all without asking permission.


