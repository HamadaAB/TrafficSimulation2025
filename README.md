![alt text](https://upload.wikimedia.org/wikipedia/commons/e/e3/Universiteit_Antwerpen_logo.svg)

# TrafficSimulation 2025

Project Software Engineering 2025. University of Antwerp.\
Made by: **Hamada Abuarab**, **Mohammed Al-Attabi** and **Alexander Matheï**

**C** = Implemented\
**X** = Not implemented\
**P** = Partially implemented\
**T** = TODO

### Overview

| Case     | Input                             | Priority  | Status |
| -------- | --------------------------------- | --------- | ------ |
| ***1:*** | ***Input***                       |           |        |
| 1.1      | Read traffic situation            | Required  | P      |
| 1.2      | Read vehicle generators           | Important | X      |
| ***2:*** | ***Output***                      |           |        |
| 2.1      | Simple output                     | Required  | T      |
| ***3:*** | ***Simulation***                  |           |        |
| 3.1      | Driving of vehicle                | Required  | T      |
| 3.2      | Simulation of traffic light       | Required  | T      |
| 3.3      | Automatic Simulation              | Required  | T      |
| 3.4      | Simulation with vehicle generator | Important | X      |

---

## Project Setup & Collaboration Guide

### Getting Started

#### 1. Clone the Repository

To get started, clone the project from GitHub using:

```sh
git clone https://github.com/YOUR_USERNAME/YOUR_REPOSITORY.git
```

#### 2. Open in CLion

- Open **CLion**.
- Click **File** → **Open** and select the cloned project folder.

#### 3. Set Up Git in CLion

- Go to **Settings** → **Version Control** → **Git**.
- Ensure Git is installed and properly configured.

### Working on the Project

#### 1. Pull the Latest Changes

Before making any changes, always pull the latest version of the project to avoid conflicts:

```sh
git pull origin main
```

#### 2. Make Your Changes

- Edit the files as needed.
- Test your code before committing.

#### 3. Commit and Push Your Changes

After making changes, run:

```sh
git add .
git commit -m "Description of changes"
git push origin main
```

#### 4. Resolving Merge Conflicts

If you run into merge conflicts:

1. Open the conflicting files.
2. Manually resolve conflicts.
3. Stage the resolved files using:
   ```sh
   git add .
   ```
4. Commit the resolved changes:
   ```sh
   git commit -m "Resolved merge conflicts"
   ```
5. Push to GitHub:
   ```sh
   git push origin main
   ```

### Additional Notes

- Always pull changes before starting new work.
- Keep commit messages clear and descriptive.
- Use branches for major changes (`git checkout -b feature-branch`).

Happy coding! 🚀

