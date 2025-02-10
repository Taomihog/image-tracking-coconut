To set up an FTP client and transfer entire folders between your Raspberry Pi 4 and a Windows host, here’s a straightforward guide:

---

### **Step 1: Set Up an FTP Server on Raspberry Pi**

1. **Update and install vsftpd (FTP server):**
   ```bash
   sudo apt update
   sudo apt install vsftpd
   ```

2. **Backup and edit the configuration file:**
   ```bash
   sudo cp /etc/vsftpd.conf /etc/vsftpd.conf.bak
   sudo nano /etc/vsftpd.conf
   ```

3. **Make these changes in `vsftpd.conf`:**
   - Uncomment or add the following lines:
     ```
     write_enable=YES
     local_umask=022
     chroot_local_user=YES
     allow_writeable_chroot=YES
     ```

4. **Restart the FTP service:**
   ```bash
   sudo systemctl restart vsftpd
   sudo systemctl enable vsftpd
   ```

5. **Create an FTP user if needed:**
   ```bash
   sudo adduser ftpuser
   ```

6. **Grant permissions for a folder (replace `ftpuser` and `/home/pi/myfolder` with your desired paths):**
   ```bash
   sudo chown ftpuser:ftpuser /home/pi/myfolder
   ```

---

### **Step 2: Install an FTP Client on Windows**
1. **Download FileZilla (recommended):**  
   - [FileZilla Client](https://filezilla-project.org/) is a popular choice.

2. **Install and open FileZilla.**

---

### **Step 3: Connect to Raspberry Pi from Windows**
1. In FileZilla:
   - Host: `<Your Raspberry Pi's IP address>`
   - Username: `ftpuser` (or your Raspberry Pi user)
   - Password: `<your password>`
   - Port: `21`
   
2. Click **Quickconnect**.

---

### **Step 4: Transfer an Entire Folder**
1. Navigate to the folder on your Raspberry Pi in FileZilla.
2. Drag and drop the folder from the Pi to your Windows machine.
3. FileZilla will handle the recursive file transfer.

---

### **Optional: Command Line FTP Transfer**
If you prefer a command-line approach on Windows:

1. Open PowerShell.
2. Use the built-in `ftp` client:
   ```powershell
   ftp <Pi-IP>
   ```
3. Enter your username and password.
4. Use commands like:
   ```
   binary
   mget <folder_name>
   quit
   ```

This setup should help you efficiently transfer folders between your Raspberry Pi and Windows PC. Let me know if you hit any snags!
