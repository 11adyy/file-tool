# file-tool

A small C++ file-transfer support library built around chunked file storage and a collector that assembles incoming pieces. The code keeps each file in memory while chunks arrive, tracks offsets, and exposes synchronization operations for coordinating reads and saves.

## Components

### `ChunkedFile`

`ChunkedFile` owns a byte buffer sized for one expected file. `GetBody()` and `GetBodySize()` expose the buffer and its capacity, and `SaveFile(path)` writes the assembled bytes to a destination. The class also exposes independent lock operations for general access and saving, including non-blocking lock checks.

### `FileCollector`

`FileCollector` indexes active files by an integer ID. `CollectFile(id, size)` creates storage for a transfer, `OnNewChunk(id, offset, chunk)` copies a received chunk into its assigned position, and `GetFile(id)` returns the current file object for further inspection or saving.

This separation lets networking code own transport and protocol details while the collector handles the in-memory assembly of file data.

## Build

The repository uses a Makefile and requires a C++ compiler with C++11 atomics and standard-library support.

```bash
make
```

The test target exercises file collection and chunk handling:

```bash
make test
```

Use `make clean` to remove generated binaries and object files. Check the Makefile for the exact compiler flags used by the current checkout.

## Basic usage

The API is declared in `include/ChunkedFile.hpp` and `include/FileCollector.hpp`. A caller can register the expected size, submit data chunks as they are received, then retrieve the file and save it once the transfer is complete. Chunk offsets are byte offsets from the start of the file.

```cpp
FileCollector collector;
collector.CollectFile(file_id, expected_size);
collector.OnNewChunk(file_id, offset, std::move(chunk));

if (ChunkedFile* file = collector.GetFile(file_id)) {
    file->SaveFile(output_path);
}
```

The example shows the intended sequence; the application should validate IDs, offsets, chunk lengths, and completion before saving. The library is a low-level building block and does not define a network protocol, retry policy, or persistent transfer database.

## Source layout

- `include/` contains the public class declarations.
- `src/` contains the implementation.
- `test/` contains the file collector test program.
- `main.cpp` is a small executable entry point for the project.
