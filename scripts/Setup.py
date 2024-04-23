import os
import subprocess
import CheckPython
import Vulkan

CheckPython.ValidatePackages()

import colorama
from colorama import Fore
from colorama import Back
from colorama import Style

os.chdir('../')

colorama.init()

if (not Vulkan.CheckVulkanSDK()):
    print("Vulkan SDK not installed!")
    exit()

if (Vulkan.CheckVulkanSDKDebugLibs()):
    print(f"{Style.BRIGHT}{Back.GREEN}Vulkan SDK debug libs located.{Style.RESET_ALL}")

print(f"{Style.BRIGHT}{Back.GREEN}Generating Visual Studio 2022 solution.{Style.RESET_ALL}")
subprocess.call(["premake/premake5.exe", "vs2022"])