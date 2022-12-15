param(
    [Parameter()]
    [switch] $Clean = $false,
    [switch] $Configure = $false,
    [switch] $Launch = $false,
    [switch] $Build = $false,
    [switch] $OpenGL = $false,
    [switch] $GLFW = $false
)

function Run {
    param (
        [Parameter()]
        [string] $Cmd,
        [string[]] $ArgumentList
    )
    
    $output = $Cmd 
    $output += " "
    $output += $ArgumentList 
    Write-Output $output
    Start-Process $Cmd -Wait -ArgumentList $ArgumentList -NoNewWindow
}

function Launch {
    Run "$PWD\bin\Debug-windows-x86_64\OpenGL\OpenGL.exe"
}

function Configure {
    Run "$PWD\vendor\bin\premake\premake5" 'vs2022'
}

function Clean {
    Remove-Item -Recurse -Force .\bin
    Remove-Item -Recurse -Force .\bin-int
}

function GLFW {
    Run "devenv" "HelloOpenGL.sln", "/Build", 'Debug', "/Project", "$PWD\vendor\glfw\glfw.vcxproj", "/Projectconfig", "Debug"
}
function OpenGL {
    Run "devenv" "HelloOpenGL.sln", "/Build", 'Debug', "/Project", "$PWD\OpenGL\OpenGL.vcxproj", "/Projectconfig", "Debug"
}


if ($Clean.IsPresent) {
    Clean
}

if ($Configure.IsPresent) {
    Configure
}

if ($Build.IsPresent) {
    GLFW
    OpenGL
} else {
    if ($GLFW.IsPresent) {
        GLFW
    }
    if ($OpenGL.IsPresent) {
        OpenGL
    }
}

if ($Launch.IsPresent) {
    Launch
}
