import metamake
import os

def main():
    import this_project
    makefile = metamake.Makefile()

    this_project.configure(makefile, "", "bin")

    makefile.dump()


if __name__ == "__main__":
    main()

