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

To get started, clone the project from GitHub using one of these methods:

**Option A: HTTPS**
```sh
git clone https://github.com/YOUR_USERNAME/YOUR_REPOSITORY.git
```

**Option B: SSH (Recommended)**
```sh
git clone git@github.com:YOUR_USERNAME/YOUR_REPOSITORY.git
```

#### Setting up SSH for GitHub (If not already configured)

**🔹 Step 1: Check for an Existing SSH Key**
Before creating a new key, check if you already have one:

```sh
ls ~/.ssh/id_rsa.pub
```
* If the file **exists**, you already have an SSH key. You can use it or create a new one.
* If it **does not exist**, proceed to Step 2.

**🔹 Step 2: Generate a New SSH Key**
Run this command in your terminal:

```sh
ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
```
* Replace `"your_email@example.com"` with your GitHub email.
* When asked for a file name, press **Enter** to save it as the default (`~/.ssh/id_rsa`).
* When asked for a **passphrase**, you can press Enter to skip (optional but more secure).

**🔹 Step 3: Add the SSH Key to GitHub**
1. Copy your public key to the clipboard using this command:

```sh
cat ~/.ssh/id_rsa.pub
```
2. Go to **GitHub** → **Settings** → **SSH and GPG keys**.
3. Click **New SSH Key**.
4. Paste the key into the box and give it a name (e.g., "My Laptop").
5. Click **Add SSH Key**.

**🔹 Step 5: Test the Connection**
Run this command to verify your SSH setup:

```sh
ssh -T git@github.com
```
* If it says **"Hi YOUR_USERNAME! You've successfully authenticated."**, you're good to go! 🎉
* If there's an error, let me know, and I'll help troubleshoot!

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


Happy coding! 🚀
