import os
import subprocess
import sys
import requests
import time
from pathlib import Path

from io import BytesIO
import colorama
from colorama import Fore
from colorama import Back
from colorama import Style

ParentPath = Path(__file__).resolve().parent
DownloadPath = ParentPath / 'downloads'

VulkanSDK = os.getenv("Vulkan_SDK")
RequiredVulkanVersion = '1.3.'
InstallVulkanVersion = '1.3.280.0'
VulkanSDKInstallerURL = f'https://sdk.lunarg.com/sdk/download/{InstallVulkanVersion}/windows/VulkanSDK-{InstallVulkanVersion}-Installer.exe'
VulkanSDKLocalPath = DownloadPath / 'VulkanSDK'
VulkanSDKExePath = VulkanSDKLocalPath / 'VulkanSDK.exe'

colorama.init()

def DownloadFile(url, filepath):
    with open(filepath, 'wb') as f:
        print("等待响应...")
        response = requests.get(url, stream=True)
        total = response.headers.get('content-length')
        print("下载中")
        if total is None:
            f.write(response.content)
        else:
            downloaded = 0
            total = int(total)
            startTime = time.time()
            for data in response.iter_content(chunk_size=max(int(total/1000), 1024*1024)):
                downloaded += len(data)
                f.write(data)
                done = int(50*downloaded/total)
                percentage = (downloaded / total) * 100
                elapsedTime = time.time() - startTime
                averageKBPerSecond = (downloaded / 1024) / elapsedTime
                averageSpeedString = '{:.2f} KB/s'.format(averageKBPerSecond)
                if (averageKBPerSecond > 1024):
                    averageMBPerSecond = averageKBPerSecond / 1024
                    averageSpeedString = '{:.2f} KB/s'.format(averageMBPerSecond)
                sys.stdout.write('\r[{}{}] {:.2f}% ({})     '.format('█' * done, '.' * (50-done), percentage, averageSpeedString))
                sys.stdout.flush()

def Judge():
    while True:
        reply = str(input('Y/N: ')).lower().strip()
        if reply[:1] == 'y':
            return True
        if reply[:1] == 'n':
            return False

def InstallVulkanSDK():
    VulkanSDKLocalPath.mkdir(parents=True, exist_ok=True)
    print(f'Downloading {VulkanSDKInstallerURL} to {VulkanSDKExePath}')
    DownloadFile(VulkanSDKInstallerURL, VulkanSDKExePath)
    print("Running Vulkan SDK installer...")
    print(f"{Style.BRIGHT}{Back.YELLOW}Make sure to install shader debug libs if you want to build in Debug!")
    os.startfile(os.path.abspath(VulkanSDKExePath))
    print(f"{Style.BRIGHT}{Back.RED}Re-run this script after installation{Style.RESET_ALL}")

def InstallVulkanPrompt():
    print("Would you like to install the Vulkan SDK?")
    install = Judge()
    if (install):
        InstallVulkanSDK()
        quit()

def CheckVulkanSDK():
    if(VulkanSDK is None):
        print(f"{Style.BRIGHT}{Back.RED}You don't have the Vulkan SDK installed!{Style.RESET_ALL}")
        InstallVulkanPrompt()
        return False
    elif (RequiredVulkanVersion not in VulkanSDK):
        print(f"Located Vulkan SDK at {VulkanSDK}")
        print(f"{Style.BRIGHT}{Back.RED}You don't have the correct Vulkan SDK verstion! (Required {RequiredVulkanVersion}){Style.RESET_ALL}")
        InstallVulkanPrompt()
        return False
    
    print(f"{Style.BRIGHT}{Back.GREEN}Correct Vulkan SDK located at {VulkanSDK}{Style.RESET_ALL}")
    return True

def CheckVulkanSDKDebugLibs():
    shadercdLib = Path(f"{VulkanSDK}/Lib/shaderc_sharedd.lib")
    if (not shadercdLib.exists()):
        print(f"{Style.BRIGHT}{Back.YELLOW}Warning: No Vulkan SDK debug libs found. (Check {shadercdLib})")
        print(f"{Back.RED}Debug builds are not possible.{Style.RESET_ALL}")
        return False
    
    return True