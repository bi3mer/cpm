# Commandline Password Manager (CPM)

Yes, command line is two words not one, but CLPM feels like too much. Anyways, I'm tired of using other people's stuf fhat I can't modify, so this is a password manager. The problem with it is that it is incredibly insecure.

## Compiling

```
make compile
```

## Set Up

You need to set the environment variable `cmppath`. This sets where the "encrypted" info is stored.

```
export cmppath=$HOME/not_very_secure.csv
```

## Running

I've updated my `$PATH` to the compiled binary, so I can run it from anywhere when I'm using my terminal. Note that sudo is required for the binary to run. The `-E` allows the root user access to local environment variables.

```
sudo -E ./cmp {cmd}
```

`{cmd}` can be the following.

- `--list`: list keys  
- `--get {key}`: get password associated with a given key 
- `--add {key} {pass}`: store key and password, will automatically overwrite previous key and password if previously stored 

# Reminder

This is not secure.
