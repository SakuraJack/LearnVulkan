import subprocess

def InstallPackage(package):
    print(f"正在安装 {package} 模块...")
    subprocess.check_call(['python', '-m', 'pip', 'install', package])

InstallPackage('setuptools')

import pkg_resources

def ValidatePackage(package):
    required = { package }
    installed = { pkg.key for pkg in pkg_resources.working_set }
    missing = required - installed

    if missing:
        InstallPackage(package)
        
def ValidatePackages():
    ValidatePackage('requests')
    ValidatePackage('colorama')