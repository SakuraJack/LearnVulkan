import os
import sys
import subprocess
import pkg_resources
import tkinter as tk
from tkinter import filedialog

def InstallModule(module):
    print(f"Installing {module} module...")
    subprocess.check_call([sys.executable, "-m", "pip", "install", module])

def ValidateRequiredModule(module):
    required = { module }
    installed = { pkg.key for pkg in pkg_resources.working_set }
    missing = required - installed

    if missing:
        InstallModule(module)

glsl_path = os.getenv("Vulkan_SDK") + "/Bin/glslc.exe"
glsl_validator_path = os.getenv("Vulkan_SDK") + "/Bin/glslangValidator.exe"

def GetFileExtension(fileName):
    return os.path.splitext(fileName)[1].lstrip(".")

def GetFileNameWithoutExtension(fileName):
    return os.path.splitext(fileName)[0]

def GetFileDirectory(fileName):
    return os.path.dirname(fileName)

def CompileShaders(shaderPaths):
    for shaderPath in shaderPaths:
        CompileShader(shaderPath)

def CompileShader(shader_path):
    fileDir = GetFileDirectory(shader_path)
    fileName = os.path.basename(shader_path)
    extension = GetFileExtension(fileName)
    fileNameWithoutExtension = GetFileNameWithoutExtension(fileName)
    switcher = {
        "vert": "_vert.spv",
        "frag": "_frag.spv",
        "comp": "_comp.spv",
    }
    print("Compiling shader: " + shader_path)
    subprocess.run([glsl_path, shader_path, "-o", fileDir + "/" + fileNameWithoutExtension + switcher.get(extension)])
    #subprocess.run([glsl_validator_path, "-V", shader_path, "-o", fileDir + "/" + fileNameWithoutExtension + switcher.get(extension)])

def ChooseShader():
    root = tk.Tk()
    root.withdraw()
    filePath = filedialog.askopenfilenames()
    return filePath

def Start():
    shaderPaths = ChooseShader()
    CompileShaders(shaderPaths)

Start()