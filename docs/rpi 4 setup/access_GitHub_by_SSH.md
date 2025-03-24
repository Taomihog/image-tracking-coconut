# Use SSH instead of a Git credential manager to push changes to GitHub from your Raspberry Pi

## 1. Generate an SSH Key

On your Raspberry Pi terminal, generate an SSH key pair:

```bash
ssh-keygen -t ed25519 -C "your_email@example.com"
```

- If `ed25519` is unsupported, you can use `rsa`

  ```bash
  ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
  ```

- When prompted:
  - Press **Enter** to accept the default file location (`~/.ssh/id_ed25519`).
  - Set a passphrase or leave it empty.

---

## 2. Add Your SSH Key to GitHub

1. Display the public key:

   ```bash
   cat ~/.ssh/id_ed25519.pub
   ```

2. Copy the entire output.

3. Go to GitHub:
   - Navigate to **Settings > SSH and GPG keys** (<https://github.com/settings/keys>, not the deploy key!)
   - Click **New SSH key**.
   - Paste the public key and give it a title.
   - Save the key.

---

## 3. Configure Git to Use SSH

Ensure Git is set to use SSH instead of HTTPS:

```bash
git remote set-url origin git@github.com:username/repository.git
```

Replace `username/repository` with your actual GitHub username and repo name.

---

## 4. Test the SSH Connection

Run this command to verify the SSH setup:

```bash
ssh -T git@github.com
```

If everything is configured correctly, you'll see:

```bash
Hi username! You've successfully authenticated, but GitHub does not provide shell access.
```

- **A potential issue on rpi4 and fix:**

when running the above command, there might be an error like this:

```bash
/etc/ssh/ssh_config: line 23: Bad configuration option: stricthostkeycheck
/etc/ssh/ssh_config: terminating, 1 bad configuration options
```

Edit the SSH Configuration File: Open the SSH config file in your favorite text editor:

```bash
sudo vim /etc/ssh/ssh_config
```

Locate the Incorrect Line: Find the line that says:

```bash
stricthostkeycheck
```

Correct it to:

```bash
StrictHostKeyChecking
```

---

## 5. Push Changes

Now you can push changes as usual:

```bash
git add .
git commit -m "Your commit message"
git push origin main
```
