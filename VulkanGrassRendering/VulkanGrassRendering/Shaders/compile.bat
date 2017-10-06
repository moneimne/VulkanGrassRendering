%VK_SDK_PATH%\Bin\glslangValidator.exe -V Graphics/graphics.vert
move vert.spv graphicsVert.spv
%VK_SDK_PATH%\Bin\glslangValidator.exe -V Graphics/graphics.frag
move frag.spv graphicsFrag.spv

%VK_SDK_PATH%\Bin\glslangValidator.exe -V grass.vert
move vert.spv grassVert.spv
%VK_SDK_PATH%\Bin\glslangValidator.exe -V grass.frag
move frag.spv grassFrag.spv
%VK_SDK_PATH%\Bin\glslangValidator.exe -V grass.tesc
move tesc.spv grassTesc.spv
%VK_SDK_PATH%\Bin\glslangValidator.exe -V grass.tese
move tese.spv grassTese.spv

%VK_SDK_PATH%\Bin\glslangValidator.exe -V compute.comp
pause