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

def CompileShaders(shaderPaths):
    for shaderPath in shaderPaths:
        CompileShader(shaderPath, shaderPath + ".spv")

def CompileShader(shader_path, output_path):
    print("Compiling shader: " + shader_path)
    subprocess.run([glsl_path, shader_path, "-o", output_path])

def ChooseShader():
    root = tk.Tk()
    root.withdraw()
    filePath = filedialog.askopenfilenames()
    return filePath

def Start():
    shaderPaths = ChooseShader()
    CompileShaders(shaderPaths)

Start()