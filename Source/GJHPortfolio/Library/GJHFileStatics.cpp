#include "GJHFileStatics.h"

void UGJHFileStatics::ForeachFileInDirectory(const FString& InDirectory, TFunctionRef<void(const FString&)> InFunc)
{
	const FString Directory = FPaths::ProjectContentDir() + InDirectory;
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.DirectoryExists(*Directory))
	{
		class FFileVisitor : public IPlatformFile::FDirectoryVisitor
		{
		private:
			TFunctionRef<void(const FString&)> Callback;
			
		public:
			FFileVisitor(TFunctionRef<void(const FString&)> InCallback)
			: Callback(InCallback)
			{}
			
		public:
			virtual bool Visit(const TCHAR* FileNameOrDirectory, bool bIsDirectory) override
			{
				if (!bIsDirectory)
					Callback(FString(FileNameOrDirectory));
				
				return true;
			}
		};

		FFileVisitor Visitor(InFunc);
		// 4. 폴더 순회 시작
		PlatformFile.IterateDirectory(*Directory, Visitor);
	}
}
