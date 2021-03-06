/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_DRM_H_
#define ANDROID_DRM_H_

#include "drmcompositor.h"
#include "drmconnector.h"
#include "drmcrtc.h"
#include "drmencoder.h"
#include "drmplane.h"

#include <stdint.h>

namespace android {

class DrmResources {
 public:
  typedef std::vector<DrmConnector *>::const_iterator ConnectorIter;
  typedef std::vector<DrmPlane *>::const_iterator PlaneIter;

  DrmResources();
  ~DrmResources();

  int Init();

  int fd() const;

  ConnectorIter begin_connectors() const;
  ConnectorIter end_connectors() const;
  PlaneIter begin_planes() const;
  PlaneIter end_planes() const;

  DrmConnector *GetConnectorForDisplay(int display) const;
  DrmCrtc *GetCrtcForDisplay(int display) const;
  DrmPlane *GetPlane(uint32_t id) const;
  DrmCompositor *compositor();

  int GetPlaneProperty(const DrmPlane &plane, const char *prop_name,
                       DrmProperty *property);
  int GetCrtcProperty(const DrmCrtc &crtc, const char *prop_name,
                      DrmProperty *property);
  int GetConnectorProperty(const DrmConnector &connector, const char *prop_name,
                           DrmProperty *property);

  uint32_t next_mode_id();
  int SetDisplayActiveMode(int display, const DrmMode &mode);
  int SetDpmsMode(int display, uint64_t mode);

  int CreatePropertyBlob(void *data, size_t length, uint32_t *blob_id);
  int DestroyPropertyBlob(uint32_t blob_id);

 private:
  int TryEncoderForDisplay(int display, DrmEncoder *enc);
  int GetProperty(uint32_t obj_id, uint32_t obj_type, const char *prop_name,
                  DrmProperty *property);

  int CreateDisplayPipe(DrmConnector *connector);

  int fd_;
  uint32_t mode_id_;

  std::vector<DrmConnector *> connectors_;
  std::vector<DrmEncoder *> encoders_;
  std::vector<DrmCrtc *> crtcs_;
  std::vector<DrmPlane *> planes_;
  DrmCompositor compositor_;
};
}

#endif  // ANDROID_DRM_H_
