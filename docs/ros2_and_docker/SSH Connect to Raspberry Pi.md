# Connect to Raspberry Pi

ssh taomihog@10.0.0.184

understand what is a server and what is a client.

understand how does ssh public key authentication work. This is exactly like how the github server works.

basic command:

```bash
# generate a key
ssh-keygen -t ed25519
# send it to server
ssh-copy-id -i path_to_pub_key_file user@ip
```

make sure the server's sshd is enabled and started (systemctl enable, start, status coomands)
