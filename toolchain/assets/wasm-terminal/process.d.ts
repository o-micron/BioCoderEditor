/// <reference types="node" />
import { WasmFs } from "@wasmer/wasmfs";
import { IoDevices } from "@wasmer/io-devices";
import CommandOptions from "../command/command-options";
import Command from "../command/command";
import IoDeviceWindow from "../io-device-window/io-device-window";
export default class Process {
    commandOptions: CommandOptions;
    wasmFs: WasmFs;
    ioDevices: IoDevices;
    originalWasmFsJson: any;
    dataCallback: Function;
    endCallback: Function;
    errorCallback: Function;
    ioDeviceWindow: IoDeviceWindow;
    sharedIoDeviceInput?: Int32Array;
    sharedStdin?: Int32Array;
    startStdinReadCallback?: Function;
    pipedStdin: string;
    stdinPrompt: string;
    readStdinCounter: number;
    command: Command;
    constructor(commandOptions: CommandOptions, wasmFsJson: any, dataCallback: Function, endCallback: Function, errorCallback: Function, ioDeviceWindow: IoDeviceWindow, sharedIoDeviceInputBuffer?: SharedArrayBuffer, sharedStdinBuffer?: SharedArrayBuffer, startStdinReadCallback?: Function);
    start(pipedStdinData?: Uint8Array): Promise<void>;
    stdoutWrite(stdoutBuffer: Buffer | Uint8Array, offset?: number, length?: number, position?: number): number;
    stdinRead(stdinBuffer: Buffer | Uint8Array, offset?: number, length?: number, position?: number): number;
}
