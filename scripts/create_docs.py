from github_tools import GitHubLogger

import subprocess
import shutil

def main():
    GitHubLogger.info("Building docs..")
    subprocess.check_call("doxygen docs/Doxyfile", shell=True)

    shutil.rmtree("build/docs/html/data")
    shutil.rmtree("build/docs/html/scripts")

    shutil.copytree("docs/data", "build/docs/html/data")
    shutil.copytree("docs/scripts", "build/docs/html/scripts")

if __name__ == "__main__":
    main()