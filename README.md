# Launch and Bounce

![Launch Bounce Icon](https://github.com/Therena/LaunchBounce/blob/main/LaunchBounce/Source/LaunchBounce.png?raw=true)

Introducing Launch and Bounce: Your go-to launcher app for executing custom command-line scripts seamlessly in the background. Track the output effortlessly with detailed logging capabilities. Say goodbye to manual monitoring and hello to efficient script execution. Streamline your workflow with Launch and Bounce.

## Usage

Launch and Bounce simplifies executing custom processes with ease. Below is a breakdown of its usage:

Process: Specify the process you want to execute using the process parameter. This can be a command-line application or a script.
```
LaunchBounce.exe -Process <your_process>
```

Parameters: If your process requires additional parameters, simply include them after specifying the process.
```
LaunchBounce.exe -Process <your_process> -Parameter <your_parameters>
```

Hide Output: To run the process in the background and hide its output, use the hide option.
```
LaunchBounce.exe -Process <your_process> -Parameter <your_parameters> -Hide 1
```
This will execute the process discreetly without displaying its window.

Stay productive and efficient with Launch and Bounce!

## License

[Apache 2.0](https://github.com/Therena/LaunchBounce/blob/master/LICENSE)
