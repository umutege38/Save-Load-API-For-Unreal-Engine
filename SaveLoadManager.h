#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SaveLoadManager.generated.h"


/**
 * \enum ESaveFileFormat
 * \brief An enumeration that represents the file format to be used for saving data.
 *
 * This enumeration defines the different file formats that can be used for saving data. Each format has a display name and a tooltip description providing information about it.
 *
 * Enum Values:
 * - BIN: Binary format, a compact and universally recognized file format that stores information in a binary form. It is suitable for saving complex data structures and is easy to use
 * with serialization/deserialization libraries.
 * - SAV: Save format, often used in video games, it is a proprietary file format that typically contains a serialized data structure. Each game or application might use this extension
 * in a different way.
 * - DAT: Data format, a generic data file with a .dat file extension. It can be in ASCII, binary or other data formats. Can be used in many applications or video games, each situating
 * their own structure or usage.
 * - JSON: JSON (JavaScript Object Notation) format, a lightweight data-interchange format.
 */
UENUM(BlueprintType)
enum class ESaveFileFormat :uint8
{
	BIN UMETA(DisplayName="Binary", Tooltip="The Binary (BIN) format is a compact, universally recognized file format that stores information in a binary form. Suited for saving complex data structures and easy to use with serialization/deserialization libraries."),
	SAV UMETA(DisplayName="Save", Tooltip="The Save (SAV) format, often used in video games, is a proprietary file format that typically contains a serialized data structure. Each game or application might use this extension in a different way."),
	DAT UMETA(DisplayName="Data", Tooltip="The Data (DAT) format is a generic data file with a .dat file extension. It can be in ASCII, binary or other data formats. Can be used in many applications or video games, each situating their own structure or usage."),
	JSON UMETA(DisplayName="JSON", Tooltip="JSON (JavaScript Object Notation) is a lightweight data-interchange format.")
};

/**
 * \brief An enumeration representing different variable data types in Unreal Engine.
 *
 * This enumeration represents the different variable data types available in Unreal Engine. It is used to define the type of data stored in a variable.
 */
UENUM(BlueprintType)
enum class EDataType : uint8
{
	FloatType     UMETA(DisplayName = "Float", Tooltip="Represents a floating-point number, used for precise calculations."),
	BoolType      UMETA(DisplayName = "Bool", Tooltip="Represents a boolean type, used for true/false conditions."),
	IntType       UMETA(DisplayName = "Int", Tooltip="Represents an integer number, used for whole number calculations."),
	FStringType   UMETA(DisplayName = "FString", Tooltip="Represents a string in Unreal Engine, used for text-based data."),
	EnumType      UMETA(DisplayName = "Enum", Tooltip="Represents an enumeration, used to define a type of element that consists of named constants."),
	ActorType     UMETA(DisplayName = "Actor", Tooltip="Represents an Actor, which is an object that exists and performs actions in the Unreal Engine world."),
	VectorType    UMETA(DisplayName = "Vector", Tooltip="Represents a Vector, which is a structure used to hold a three-dimensional point such as position or direction."),
	RotatorType   UMETA(DisplayName = "Rotator", Tooltip="Represents a Rotator, which is a structure used to hold rotation in 3-dimensional space."),
	TransformType UMETA(DisplayName = "Transform", Tooltip="Represents a Transform, which is used to store a combination of translation (position), rotation, and scale.")
};

/**
 * \brief A struct that represents serialized data in your project, containing data type, key, and serialized data.
 *
 * This struct is used to store serialized data, including the data type, key, and the actual serializable data.
 */
USTRUCT(Meta = (ToolTip = "A struct that represents serialized data in your project, containing data type, key, and serialized data."))
struct FSerializedData
{
	GENERATED_BODY()

	/**
	 * \var DataType
	 * \brief Specifies the type of the data being serialized.
	 *
	 * This property is of type EDataType and is used for specifying the type of data being serialized. It is a UPROPERTY, which means it can be exposed to the editor with meta information
	 *. The Meta attribute is used to provide additional information about the property, such as a tooltip that provides a brief description of its purpose.
	 */
	UPROPERTY(Meta = (ToolTip = "Specifies the type of the data being serialized."))
	EDataType DataType;

	/**
	 * \brief The key associated with this piece of serialized data.
	 *
	 * This variable represents the key associated with a piece of serialized data. It is used to identify and access the serialized data in the save file.
	 */
	UPROPERTY(Meta = (ToolTip = "The key associated with this piece of serialized data."))
	FString Key;

	/**
	 * \brief The actual serializable data stored as an array of bytes.
	 * \details This property stores the serialized data in the form of an array of bytes. The data can be accessed and manipulated using the TArray<uint8> data structure.
	 *
	 * \see TArray<uint8>
	 */
	UPROPERTY(Meta = (ToolTip = "The actual serializable data stored as an array of bytes."))
	TArray<uint8> Data;

	/**
	 
	 * \fn friend FArchive& operator<<(FArchive& Ar, FSerializedData& SerializedData)
	 * \brief Overload of the stream insertion operator (<<) for serializing data.
	 *
	 * \param Ar The archive used for serializing the data.
	 * \param SerializedData The data object to be serialized.
	 *
	 * \return A reference to the archive after serialization.
	 */
	friend FArchive& operator<<(FArchive& Ar, FSerializedData& SerializedData)
	{
		Ar << SerializedData.DataType;
		Ar << SerializedData.Key;
		Ar << SerializedData.Data;
		return Ar;
	}
};


/**
 * \class USaveLoadManager
 * \brief A class responsible for managing saving and loading data.
 */
UCLASS()
class CSS_API USaveLoadManager : public UObject
{
	GENERATED_BODY()

public:
	USaveLoadManager();

private:

	inline static FString DefaultSaveFileName = "GameSave"; // You can change this name, but make sure to delete the old save before doing so.

public:

	/**
	 * \brief Prepare the file path for saving or loading data.
	 * \param FileName The name of the file.
	 * \param SaveFileFormat The format of the save file.
	 * \return The full file path for the specified file name and format.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Prepares the full file path from a given file name and specific file format."))
	static FString PrepareFilePath(const FString& FileName, ESaveFileFormat SaveFileFormat);

	/**
	 * \brief Checks if a file exists at the given file path.
	 * \param FilePath The file path to check.
	 * \return True if the file exists, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Checks if a file exists at a given file path."))
	static bool DoesFileExist(const FString& FilePath);

	/**
	 * \brief Deletes a file with the specified file name.
	 *
	 * \param FileName The name of the file to be deleted.
	 *
	 * \return None.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Deletes a file with a given file name."))
	static void DeleteFile(const FString& FileName);

	/**
	 * \brief Returns the default file name for saving data.
	 * \return A FString representing the default file name for saving data.
	 *
	 * This method returns the default file name for saving data. The default file name is stored in the static member variable DefaultSaveFileName.
	 * This value can be changed by modifying the DefaultSaveFileName variable.
	 *
	 * Example usage:
	 * \code{.cpp}
	 * FString defaultFileName = USaveLoadManager::GetDefaultSaveFileName();
	 * \endcode
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Returns the default file name for saving data."))
	static FString GetDefaultSaveFileName() { return DefaultSaveFileName;}

	/**
	 * \brief Saves data to a file at the specified path.
	 *
	 * This method saves the given data to a file at the specified path. If the file already exists, the data will be appended to the existing data. If the file does not exist, a new file
	 * will be created.
	 *
	 * \param Key The key associated with the data. This is used to identify and retrieve the data when loading.
	 * \param Data The data to be saved. This should be an array of uint8 values.
	 * \param DataType The type of data being saved.
	 * \param SaveFilePath The path to the file where the data will be saved. This should be a valid file path including the file name and extension.
	 *
	 * \return true if the data was successfully saved, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Saves a piece of data with a key to a specific file."))
	static bool SaveData(const FString& Key, const TArray<uint8>& Data, EDataType DataType, const FString& SaveFilePath);

	/**
	 * \brief Loads data from a save file.
	 *
	 * \param Key The key used to identify the data in the save file.
	 * \param OutData The output array that will contain the loaded data.
	 * \param OutDataType The output data type of the loaded data.
	 * \param SaveFilePath The file path of the save file.
	 *
	 * \return True if the data was successfully loaded, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Loads data by given key from a specific file path into the provided output parameters."))
	static bool LoadData(const FString& Key, TArray<uint8>& OutData, EDataType& OutDataType, const FString& SaveFilePath);

	/**
	 * \brief Deletes data entry with the specified key from the save file.
	 *
	 * This method deletes a data entry with the specified key from the save file located at the provided file path. The method loads the save file into memory, removes the data entry with
	 * the specified key, and then serializes the remaining entries back to the file. If the delete operation is successful, the method returns true. Otherwise, it returns false.
	 *
	 * \param Key The key of the data entry to be deleted.
	 * \param SaveFilePath The file path to the save file.
	 *
	 * \return True if the data entry is successfully deleted, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Deletes the data associated with a given key from a specific file."))
	static bool DeleteData(const FString& Key, const FString& SaveFilePath);
	
	/**
	 * \fn TArray<uint8> FloatToByteArray(float Value)
	 * Converts a float value to a byte array.
	 * \param Value The float value to convert.
	 * \return The byte array representation of the float value.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts a float value to a byte array."))
	static TArray<uint8> FloatToByteArray(float Value)
	{
		TArray<uint8> ByteArray;
		FMemoryWriter MemoryWriter(ByteArray, true);
		MemoryWriter << Value;
		return ByteArray;
	}

	/**
	 * \fn float ByteArrayToFloat(const TArray<uint8>& ByteArray)
	 * \brief Converts a byte array to a float value.
	 * \param ByteArray The byte array to be converted to a float value.
	 * \return The resulting float value from the byte array.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts a byte array to a float value."))
	static float ByteArrayToFloat(const TArray<uint8>& ByteArray)
	{
		float Value = 0.0f;
		FMemoryReader MemoryReader(ByteArray, true);
		MemoryReader << Value;
		return Value;
	}

	/**
	 * \brief Converts a boolean value to a byte array.
	 * \param Value The boolean value to convert.
	 * \return A byte array representation of the boolean value.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts a boolean value to a byte array."))
	static TArray<uint8> BoolToByteArray(bool Value)
{
    TArray<uint8> ByteArray;
    FMemoryWriter MemoryWriter(ByteArray, true);
    MemoryWriter << Value;
    return ByteArray;
}

	/**
	 * \brief Converts a byte array to a boolean value.
	 * \param ByteArray The array of bytes to convert to a boolean value.
	 * \return The boolean value converted from the byte array.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts a byte array to a boolean value."))
	static bool ByteArrayToBool(const TArray<uint8>& ByteArray)
	{
		bool Value = false;
		FMemoryReader MemoryReader(ByteArray, true);
		MemoryReader << Value;
		return Value;
	}

	/**
	 * \brief Converts an integer value to a byte array.
	 * \param Value The integer value to be converted.
	 * \return The byte array representation of the integer value.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts an integer value to a byte array."))
	static TArray<uint8> IntToByteArray(int32 Value)
	{
		TArray<uint8> ByteArray;
		FMemoryWriter MemoryWriter(ByteArray, true);
		MemoryWriter << Value;
		return ByteArray;
	}

	/**
	 * \brief Converts a byte array to an integer value.
	 * \param ByteArray The byte array to convert.
	 * \return The converted integer value.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts a byte array to an integer value."))
	static int32 ByteArrayToInt(const TArray<uint8>& ByteArray)
	{
		int32 Value = 0;
		FMemoryReader MemoryReader(ByteArray, true);
		MemoryReader << Value;
		return Value;
	}

	/**
	 * \brief Converts a FString value to a byte array.
	 * \param Value The FString value to convert.
	 * \return The byte array representation of the FString value.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts a FString value to a byte array."))
	static TArray<uint8> FStringToByteArray(FString& Value)
{
    TArray<uint8> ByteArray;
    FMemoryWriter MemoryWriter(ByteArray, true);
    MemoryWriter << Value;
    return ByteArray;
}

	/**
	 * \brief Converts a byte array to a FString value.
	 * \param ByteArray The byte array to convert.
	 * \return The FString value converted from the byte array.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts a byte array to a FString value."))
	static FString ByteArrayToFString(const TArray<uint8>& ByteArray)
{
    FString Value;
    FMemoryReader MemoryReader(ByteArray, true);
    MemoryReader << Value;
    return Value;
}

	/**
	 * \brief Converts an enumeration value to a byte array.
	 *
	 * This method takes an enumeration value as input and converts it to a byte array representation.
	 * The byte array can be used for saving or transmitting the enumeration value.
	 *
	 * \param EnumValue The enumeration value to convert to a byte array.
	 *
	 * \return A byte array representation of the given enumeration value.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts an enumeration value to a byte array."))
	static TArray<uint8> EnumToByteArray(uint8 EnumValue)
	{
		TArray<uint8> ByteArray;
		FMemoryWriter MemoryWriter(ByteArray, true);
		MemoryWriter << EnumValue;
		return ByteArray;
	}

	/**
	 * @brief Converts an enum value to a byte array.
	 *
	 * This method takes an unsigned 16-bit integer representing an enum value and converts it to a byte array.
	 * The byte array will have a size equal to the size of a 16-bit uint. The enum value is copied byte by byte
	 * into the byte array using the FMemory::Memcpy function.
	 *
	 * @param EnumValue The enum value to convert.
	 * @return A byte array containing the converted enum value.
	 */
	static TArray<uint8> EnumToByteArray(uint16 EnumValue)
	{
		TArray<uint8> ByteArray;
		ByteArray.SetNumUninitialized(sizeof(uint16)); // Sets the number of uninitialized elements in array to sizeof(uint16).
		FMemory::Memcpy(ByteArray.GetData(), &EnumValue, sizeof(uint16)); // Copies the 2 bytes of EnumValue into ByteArray.
		return ByteArray;
	}

	/**
	 * \brief Converts an enumeration value to a byte array.
	 *
	 * \param EnumValue The enumeration value to be converted.
	 * \return A byte array representation of the enumeration value.
	 */
	static TArray<uint8> EnumToByteArray(uint32 EnumValue)
	{
		TArray<uint8> ByteArray;
		ByteArray.SetNumUninitialized(sizeof(uint32)); // Sets the number of uninitialized elements in array to sizeof(uint32).
		FMemory::Memcpy(ByteArray.GetData(), &EnumValue, sizeof(uint32)); // Copies the 4 bytes of EnumValue into ByteArray.
		return ByteArray;
	}

	/**
	 * \fn static TArray<uint8> EnumToByteArray(uint64 EnumValue)
	 * \brief Converts an enumeration value to a byte array.
	 *
	 * This method takes an enumeration value and converts it to a byte array representation. The resulting byte array will have a size of sizeof(uint64).
	 *
	 * \param EnumValue The enumeration value to be converted to a byte array.
	 *
	 * \return ByteArray The byte array representation of the enumeration value.
	 *
	 * Example usage:
	 * \code
	 * TArray<uint8> byteArray = EnumToByteArray(ESaveFileFormat::BIN);
	 * \endcode
	 */
	static TArray<uint8> EnumToByteArray(uint64 EnumValue)
	{
		TArray<uint8> ByteArray;
		ByteArray.SetNumUninitialized(sizeof(uint64)); // Sets the number of uninitialized elements in array to sizeof(uint64).
		FMemory::Memcpy(ByteArray.GetData(), &EnumValue, sizeof(uint64)); // Copies the 8 bytes of EnumValue into ByteArray.
		return ByteArray;
	}
	
	/**
	 * \brief Converts a byte array to an enumeration value.
	 *
	 * \param ByteArray The byte array to convert.
	 * \return The converted enumeration value.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts a byte array to an enumeration value."))
	static uint8 ByteArrayToEnum(const TArray<uint8>& ByteArray)
	{
		uint8 EnumValue = 0;
		FMemoryReader MemoryReader(ByteArray, true);
		MemoryReader << EnumValue;
		return EnumValue;
	}

	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts an FTransform to a byte array."))
	static TArray<uint8> TransformToByteArray(FTransform Value)
	{
		TArray<uint8> ByteArray;
		FMemoryWriter MemoryWriter(ByteArray, true);
		MemoryWriter << Value;
		return ByteArray;
	}

	UFUNCTION(BlueprintCallable, Category = "SaveLoad", Meta = (ToolTip = "Converts a byte array to a FTransform."))
	static FTransform ByteArrayToTransform(const TArray<uint8>& ByteArray)
	{
		FTransform Value;
		FMemoryReader MemoryReader(ByteArray, true);
		MemoryReader << Value;
		return Value;
	}
	
};

