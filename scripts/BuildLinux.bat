.nuget\nuget.exe restore stormancer-sdk-cpp-141.sln
msbuild stormancer-sdk-cpp-141.sln /t:stormancer-sdk-cpp-lib-Linux /p:Configuration=Release /p:Platform=Linux-x64
msbuild stormancer-sdk-cpp-141.sln /t:stormancer-sdk-cpp-lib-Linux /p:Configuration=Debug /p:Platform=Linux-x64