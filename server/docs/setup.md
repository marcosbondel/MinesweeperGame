# Setup
`/dev/cu.usbmodem11401`
```bash
    # Create environment
    python3 -m venv env

    # Activate your environment
    source ./env/bin/activate

    # In case, you need to deacivate it
    deactivate

    # Install packages
    pip3 install flask

    # Generate the requirements.txt
    pip freeze > requirements.txt

    # In case, you want to install the dependencies in the requirements.txt
    pip3 install -r requirements.txt
```

## Troubleshooting

In case, the port is busy
```bash
    # Check the process ID
    lsof /dev/cu.usbmodem11401

    # Kill the process
    kill -9 <PID> 
```

Check Arduino Port on MacOS
```bash
    ls /dev/cu.*
```