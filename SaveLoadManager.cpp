#include "SaveLoadManager.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Serialization/Archive.h"
#include "Serialization/BufferArchive.h"

USaveLoadManager::USaveLoadManager()
{
	
}

FString USaveLoadManager::PrepareFilePath(const FString& FileName, ESaveFileFormat SaveFileFormat)
{
	// Base directory for saved game files
	static const FString BaseDirectory = FPaths::ProjectSavedDir() + "/SavedGames";

	// Ensure the base directory exists
	if (!FPaths::DirectoryExists(BaseDirectory))
	{
		IFileManager::Get().MakeDirectory(*BaseDirectory, true);
	}

	// Determine file extension based on the save file format
	FString FileExtension;
	switch (SaveFileFormat)
	{
	case ESaveFileFormat::BIN:
		FileExtension = TEXT(".bin"); 
		break;
	case ESaveFileFormat::SAV:
		FileExtension = TEXT(".sav");
		break;
	case ESaveFileFormat::DAT:
		FileExtension = TEXT(".dat");
		break;
	default:
		// Default to ".bin" if format is unknown
			FileExtension = TEXT(".bin");
		break;
	}

	// Construct and return the full file path
	return BaseDirectory + TEXT("/") + FileName + FileExtension;
}

bool USaveLoadManager::DoesFileExist(const FString& FilePath)
{
	return FPaths::FileExists(FilePath);
}

void USaveLoadManager::DeleteFile(const FString& FileName)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.FileExists(*FileName))
	{
		const bool bSuccess = PlatformFile.DeleteFile(*FileName);
		if (!bSuccess)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to delete file: %s"), *FileName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("File does not exist: %s"), *FileName);
	}
}

bool USaveLoadManager::SaveData(const FString& Key, const TArray<uint8>& Data, EDataType DataType, const FString& SaveFilePath)
{
    TArray<uint8> ByteArray;

    // Load existing data if the file exists
    if (FPaths::FileExists(SaveFilePath))
    {
        if (FFileHelper::LoadFileToArray(ByteArray, *SaveFilePath))
        {
            FMemoryReader MemoryReader(ByteArray, true);
            TArray<FSerializedData> ExistingData;

            // Read existing serialized data
            while (!MemoryReader.AtEnd())
            {
                FSerializedData SerializedData;
                MemoryReader << SerializedData;
                ExistingData.Add(SerializedData);
            }

            // Remove existing entry with the same key
            ExistingData.RemoveAll([&Key](const FSerializedData& Entry) { return Entry.Key == Key; });

            // Add new data entry
            FSerializedData NewData;
            NewData.Key = Key;
            NewData.DataType = DataType;
            NewData.Data = Data;
            ExistingData.Add(NewData);

            // Serialize all entries back to the file
            ByteArray.Empty();
            FMemoryWriter MemoryWriter(ByteArray, true);
            for (FSerializedData& DataEntry : ExistingData)
            {
                MemoryWriter << DataEntry;
            }

            if (FFileHelper::SaveArrayToFile(ByteArray, *SaveFilePath))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        // File does not exist, create new one
        FSerializedData NewData;
        NewData.Key = Key;
        NewData.DataType = DataType;
        NewData.Data = Data;

        ByteArray.Empty();
        FMemoryWriter MemoryWriter(ByteArray, true);
        MemoryWriter << NewData;

        if (FFileHelper::SaveArrayToFile(ByteArray, *SaveFilePath))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool USaveLoadManager::LoadData(const FString& Key, TArray<uint8>& OutData, EDataType& OutDataType, const FString& SaveFilePath)
{
    if (FPaths::FileExists(SaveFilePath))
    {
        TArray<uint8> ByteArray;
        if (FFileHelper::LoadFileToArray(ByteArray, *SaveFilePath))
        {
            FMemoryReader MemoryReader(ByteArray, true);

            while (!MemoryReader.AtEnd())
            {
                FSerializedData SerializedData;
                MemoryReader << SerializedData;

                if (SerializedData.Key == Key)
                {
                    OutData = SerializedData.Data;
                    OutDataType = SerializedData.DataType;
                    return true; // Data found
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *SaveFilePath);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("File not found: %s"), *SaveFilePath);
    }

    return false; // Data not found
}

bool USaveLoadManager::DeleteData(const FString& Key, const FString& SaveFilePath)
{
	if (FPaths::FileExists(SaveFilePath))
	{
		TArray<uint8> ByteArray;
		if (FFileHelper::LoadFileToArray(ByteArray, *SaveFilePath))
		{
			TArray<FSerializedData> ExistingData;
			FMemoryReader MemoryReader(ByteArray, true);

			// Read existing serialized data
			while (!MemoryReader.AtEnd())
			{
				FSerializedData SerializedData;
				MemoryReader << SerializedData;
				ExistingData.Add(SerializedData);
			}

			// Remove data entry with the specified key
			ExistingData.RemoveAll([&Key](const FSerializedData& Entry) { return Entry.Key == Key; });

			// Serialize the remaining entries back to the file
			ByteArray.Empty();
			FMemoryWriter MemoryWriter(ByteArray, true);
            
			for (FSerializedData& DataEntry : ExistingData)
			{
				MemoryWriter << DataEntry;
			}

			if (FFileHelper::SaveArrayToFile(ByteArray, *SaveFilePath))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
