# Install docker on raspberry pi

---

## **1. Update the System**

Make sure your package list is up to date:

```bash
sudo apt-get update
sudo apt-get upgrade
```

---

## **2. Set Up Docker's Official Repository**

Add Docker's official GPG key and repository:

```bash
curl -fsSL https://download.docker.com/linux/debian/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
```

Add the Docker repository for Raspberry Pi (assuming it runs on Debian-based distributions):

```bash
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/debian \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
```

---

## **3. Install Docker**

Update the package list and install Docker:

```bash
sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

---

## **4. Verify Installation**

Check that Docker is installed and running:

```bash
sudo docker --version
sudo systemctl status docker
```

---

## **5. (Optional) Run Docker Without Sudo**

Add your user to the Docker group:

```bash
sudo usermod -aG docker $USER
```

Log out and log back in for the group changes to take effect.

---

## **Troubleshooting Tips**

- If `docker-ce` still isn't available, confirm your Raspberry Pi architecture:

  ```bash
  uname -m
  ```

- For `armhf` or `arm64`, Docker Desktop or Docker-compatible alternatives may need adjustments.

Let me know if you encounter any other issues.
