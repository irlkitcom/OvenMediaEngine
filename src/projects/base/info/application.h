//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2018 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include <base/ovcrypto/ovcrypto.h>
#include <config/config.h>

#include "stream.h"

class Orchestrator;

namespace info
{
	typedef uint32_t application_id_t;
	constexpr application_id_t InvalidApplicationId = std::numeric_limits<application_id_t>::max();
	constexpr application_id_t MinApplicationId = std::numeric_limits<application_id_t>::min();
	constexpr application_id_t MaxApplicationId = (InvalidApplicationId - static_cast<application_id_t>(1));

	class Application
	{
	public:
		bool IsValid() const
		{
			return (_application_id != InvalidApplicationId);
		}

		application_id_t GetId() const
		{
			return _application_id;
		}

		const ov::String &GetName() const
		{
			return _name;
		}

		template <typename Tpublisher>
		const Tpublisher *GetPublisher() const
		{
			Tpublisher temp_publisher;
			const auto &publishers = _app_config.GetPublishers().GetPublisherList();

			for (auto &publisher_info : publishers)
			{
				if (temp_publisher.GetType() == publisher_info->GetType())
				{
					return dynamic_cast<const Tpublisher *>(publisher_info);
				}
			}

			return nullptr;
		}

		template <typename Tprovider>
		const Tprovider *GetProvider() const
		{
			Tprovider temp_provider;
			const auto &providers = _app_config.GetProviders().GetProviderList();

			for (auto &provider_info : providers)
			{
				if (temp_provider.GetType() == provider_info->GetType())
				{
					return dynamic_cast<const Tprovider *>(provider_info);
				}
			}

			return nullptr;
		}

		const cfg::Application &GetConfig() const
		{
			return _app_config;
		}

		cfg::Application &GetConfig()
		{
			return _app_config;
		}

		bool CheckCodecAvailability(const std::vector<ov::String> &video_codecs, const std::vector<ov::String> &audio_codecs) const
		{
			for (const auto &stream : GetConfig().GetStreamList())
			{
				const auto &profiles = stream.GetProfileList();
				for (const auto &profile : profiles)
				{
					cfg::StreamProfileUse use = profile.GetUse();
					ov::String profile_name = profile.GetName();

					if (GetConfig().HasEncodeWithCodec(profile_name, use, video_codecs, audio_codecs))
					{
						return true;
					}
				}
			}

			return false;
		}

	protected:
		// These constructors will be called from Orchestrator
		friend class ::Orchestrator;
		Application(application_id_t app_id, const ov::String &name, cfg::Application app_config);
		Application(application_id_t app_id, const ov::String &name);

		// This function is created to minimize the creation of temporary instances
		static const Application &GetInvalidApplication();

		application_id_t _application_id = InvalidApplicationId;
		ov::String _name;

		cfg::Application _app_config;
	};
}  // namespace info