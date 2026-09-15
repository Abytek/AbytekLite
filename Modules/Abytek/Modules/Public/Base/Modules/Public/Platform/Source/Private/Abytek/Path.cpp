#include "Abytek/Path.hpp"
#include "Abytek/FSCommon.hpp"
#include "Abytek/FSUtilities.hpp"


namespace Abytek
{
    F_Text H_Path::Resolve(const F_Text& path, const F_Text& base_path)
	{
		if (path.size() == 0)
			return base_path;

		if (path.size() == 1)
		{
			if (path[0] == ABYTEK_TEXT('.'))
			{
				return base_path;
			}
		}
		else
		{
			if (path[0] == ABYTEK_TEXT('.') && path[1] != ABYTEK_TEXT('.'))
			{
				return base_path + ABYTEK_TEXT("/") + path.substr(1, path.size() - 1);
			}
		}

		return base_path + ABYTEK_TEXT("/") + path;
	}
	TF_Optional<F_Text> H_Path::FindAbsolutePath(const F_Text& Path, const TF_Vector<F_Text>& ExternalBasePaths, const TW<A_FSAdapter>& Adapter)
	{
		// cwd path
		if (H_FSUtilities::Exists(Path, E_FSEntryType::FILE))
		{
			return Path;
		}

		//
		for(auto& ExternalBasePath : ExternalBasePaths)
		{
			F_Text AbsolutePath = Resolve(Path, ExternalBasePath);
			if (H_FSUtilities::Exists(AbsolutePath, E_FSEntryType::FILE))
			{
				return AbsolutePath;
			}
		}
		return {};
	}
	F_Text H_Path::GetExtension(const F_Text& path)
	{
		F_Text result;

		for (size_t i = path.size() - 1; i >= 0; --i)
		{
			auto C = path[i];

			if (
				C == ABYTEK_TEXT('/')
				|| C == ABYTEK_TEXT('\\')
			)
				return ABYTEK_TEXT("");
			if(
				C == ABYTEK_TEXT('.')
				|| C == (':')
			)
				break;

			result = C + result;
		}

		return std::move(result);
	}
	F_Text H_Path::GetFileName(const F_Text& path)
	{
		auto begin = path.find_last_of(ABYTEK_TEXT("/\\"));
		if(begin != -1)
		{
			++begin;
			return path.substr(begin);
		}

		return path;
	}
	F_Text H_Path::RemoveExtension(const F_Text& path)
	{
		auto GetExtension = H_Path::GetExtension(path);

		return path.substr(0, Max(PDiff(path.size() - GetExtension.size()) - 1, PDiff(0)));
	}
	F_Text H_Path::GetBaseName(const F_Text& path)
	{
		auto end = path.find_last_of(ABYTEK_TEXT("/\\"));
		if(end != -1)
		{
			return path.substr(0, end);
		}

		return {};
	}
	TF_Vector<F_Text> H_Path::Split(const F_Text &Path)
	{
		TF_Vector<F_Text> items;
		F_Text current_items = ABYTEK_TEXT("");
		for (int32_t i = 0; i < Path.size();)
		{
			int32_t j = i;

			for (; j < Path.size();)
			{
				if (Path[j] == ABYTEK_TEXT('/') || Path[j] == ABYTEK_TEXT('\\'))
				{
					while (Path[j] == ABYTEK_TEXT('/') || Path[j] == ABYTEK_TEXT('\\'))
					{
						++j;
					}
					break;
				}

				current_items += Path[j];
				++j;
			}

			if (current_items.size() != 0) {

				items.push_back(current_items);
			}
			current_items = ABYTEK_TEXT("");

			i = j;
		}
		return std::move(items);
	}
	F_Text H_Path::Normalize(const F_Text& Path)
	{
	    if (Path.size() == 0)
	        return ABYTEK_TEXT("");

	    F_Text result;

		auto items = Split(Path);

	    int32_t back_count = 0;
	    for (ptrdiff_t i = items.size() - 1; i >= 0; --i)
	    {
	        const auto &item = items[i];

	        if (item == ABYTEK_TEXT(".."))
	        {
	            ++back_count;
	        }
	    	else
	        {
	            if (!back_count)
	            {
	                if (result.size() == 0)
	                    result = item;
	                else
	                    result = item + ABYTEK_TEXT('/') + result;
	            }
	    		else --back_count;
	        }
	    }

	    if (Path[0] == ABYTEK_TEXT('/') || Path[0] == ABYTEK_TEXT('\\'))
	        return ABYTEK_TEXT('/') + result;

	    return std::move(result);
	}
}